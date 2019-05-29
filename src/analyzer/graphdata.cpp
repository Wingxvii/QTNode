#include "graphdata.h"

#include "Logger.h"
#include <QStringList>
#include <QStringListModel>
#include <QList>
#include "linegraph.h"
#include "graphdataconnector.h"

GraphData::GraphData(QJsonArray data, QString name, Events* events)
{
    this->events = events;
    QStringListModel* model = new QStringListModel(this);


    connect(this, SIGNAL(sendData(QString, std::map<float, float>, GraphStats)),
            &GraphDataConnector::getInstance(), SLOT(addData(QString, std::map<float, float>, GraphStats)));

    //create vertical layout
    //this will contain the name and the tabWidget
    layout = new QGridLayout();
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 1);

    container = new QWidget();
    container->setLayout(layout);

    this->name = name;

    draggable = new QLabel("|||");
    layout->addWidget(draggable, 1, 0);

    tabWidget = new QTabWidget();
    tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout->addWidget(tabWidget, 1, 1);

    listWidget = new QWidget();
    scrollarea = new QScrollArea();
    listview = new QListView();

    tabWidget->addTab(listWidget, QString("Raw Data"));
    tabWidget->addTab(scrollarea, QString("Line Graph"));

    //The layout inside of our firt tab
    QVBoxLayout *layout2 = new QVBoxLayout();
    QVBoxLayout *layout3 = new QVBoxLayout();

    listWidget->setLayout(layout2);
    scrollarea->setLayout(layout3);

    //add list view to our first tab
    layout2->addWidget(listview);

    scrollarea->setWidgetResizable(true);
    //scrollarea->setWidget(lineGraph);

    QStringList keys = data.at(0).toObject().keys();
    if (keys.contains("Timestamp"))
    {
        LOG_CAMPBELL() << name + " Contains timestamp";
        model->setStringList(setupList(data));
    }
    else
    {
        QStringList compendium;
        for (int i = 0; i < data.size(); i++)
        {
            LOG_CAMPBELL() << name + " ";
            QJsonArray holder = data.at(i).toObject().find("DataArray").value().toArray();
            compendium += setupList(holder);
        }
        model->setStringList(compendium);
    }

    listview->setModel(model);
    listview->setEditTriggers(QAbstractItemView::NoEditTriggers);

    lineGraph = new LineGraph(stats);
    scrollarea->setWidget(lineGraph);
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(updateGraphs()));
}

void GraphData::setupNodeEditor(FilterNode *node)
{
    tabWidget->addTab(node, tr("NodeEditor"));
}

//creates the list that our raw data displays
QStringList GraphData::setupList(QJsonArray data)
{
    QStringList list;

    QString valueString;
    QStringList keys = data.at(0).toObject().keys();

    //loop through the keys in the map
    for (int i = 0; i < keys.size(); i++)
    {
        //make sure it isnt the timestamp
        if (keys.at(i).compare("Timestamp"))
        {
            //set the key
            valueString = keys.at(i);
            //loop through the data
            for (int i = 0; i < data.size(); i++)
            {
                //make sure the data is an object
                if (data.at(i).isObject())
                {
                    //Conversion
                    QJsonObject value = data.at(i).toObject();
                    double timeStamp = value.find("Timestamp").value().toDouble();
                    QString timeText = "Timestamp: " + QString::number(timeStamp, 'f');
                    //make sure the value exists
                    if (value.contains(valueString))
                    {
                        double yValue = value.find(valueString).value().toDouble();
                        list << timeText + ": " + valueString + ": " + QString::number(yValue, 'f');
                        sources[valueString].points[timeStamp] = yValue;
                        stats.addData(timeStamp, yValue);
                    }
                    //if it doesnt deal with the octaves cases
                    else
                    {
                        int k = 0;
                        LOG_CAMPBELL() << "Octaves have been found";
                        QString concatenate = valueString + "[" + QString::number(k) + "]";
                        while (value.contains(concatenate))
                        {
                            double yValue = value.find(valueString).value().toDouble();
                            list << timeText;
                            list << valueString + ": " + QString::number(yValue, 'f');
                            sources[valueString].points[yValue];
                            stats.addData(timeStamp, yValue);

                            k++;
                            concatenate ="Value[" + QString::number(k) + "]";
                        }
                    }
                }
                else
                {
                    LOG_CAMPBELL() << "Error at " << QString::number(i);
                }
            }
        }
    }

    for (auto data : sources)
    {
        emit sendData(data.first, data.second.points, stats);
    }
    return list;
}

QGridLayout* GraphData::getLayout()
{
    return layout;
}

QWidget* GraphData::getContainer()
{
    return container;
}

QString GraphData::getName()
{
    return name;
}

void GraphData::setDraggable(bool state)
{
    if (state)
    {
        draggable->show();
    }
    else
    {
        draggable->hide();
    }
}


void GraphData::updateGraphs()
{
    lineGraph->clearPixmap();
    if (tabWidget->currentIndex() == graphIndex)
    {
        for (auto graphs : sources)
        {
            lineGraph->updatePixmap(graphs.second.points);
        }
    }
    lineGraph->update();
}
