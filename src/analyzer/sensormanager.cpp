#include "analyzer/sensormanager.h"
#include <Logger.h>
#include <QtWidgets/QScrollArea>
#include <QAction>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <settings.h>
#include <dialogs/blacklist.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


QJsonArray getJsonArray(QJsonObject object, QString key)
{
    return object.find(key).value().toArray();
}

SensorManager::SensorManager(QWidget *parent) :
    QWidget(parent),
    sensorEvents()
{

    resize(0, 0);
    LOG_CAMPBELL() << "begin";
    model = new QStringListModel(this);

    layout = new QVBoxLayout();

    open = new QPushButton("open");
    blacklist = new QPushButton("blacklist");

    orignals = new QListView();
    virtualSource = new QListWidget();
    events = new QListWidget();

    virtualSource->addItem("+");
    //events->addItem("+");
    virtualIndex = 0;
    eventsIndex = 0;

    setLayout(layout);
    layout->addWidget(open);
    layout->addWidget(blacklist);
    layout->addWidget(orignals);
    //layout->addWidget(virtualSource);
    layout->addWidget(events);

    connect(open, SIGNAL(clicked(bool)), this, SLOT(on_open_clicked()));
    connect(blacklist, SIGNAL(clicked(bool)), this, SLOT(on_blackList_clicked()));
    connect(orignals, SIGNAL(clicked(QModelIndex)), this, SLOT(on_listView_doubleClicked(QModelIndex)));
    connect(events, SIGNAL(clicked(QModelIndex)), this, SLOT(on_events_Clicked(QModelIndex)));

    //I had to remove the idea of virtual data because i ran out of time
    //connect(virtualSource, SIGNAL(clicked(QModelIndex)), this, SLOT(on_virtual_clicked(QModelIndex)));

    sensorTime= -1;
    videoTime = -1;

    blackList = ShotcutSettings::singleton().getBlackList();
}

void SensorManager::on_open_clicked()
{
    //File choosing
    LOG_CAMPBELL() << "Sensors open button triggered";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open sensor"), "");

    //check whether file exists
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_CAMPBELL() << "Error opening file";
        return;
    }


    //read from file
    QByteArray line;
    while(!file.atEnd())
    {
        line += file.readLine();
    }

    //load it into a json object
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(line);
    if (jsonDoc.isNull())
    {
        LOG_CAMPBELL() << "Json data is null";
        return;
    }
    else if (jsonDoc.isEmpty())
    {
        LOG_CAMPBELL() << "Json data is empty";
        return;
    }

    if (jsonDoc.isArray())
    {
        LOG_CAMPBELL() << "Json data is an array";
        sensorData = jsonDoc.array();
    }
    else if (jsonDoc.isObject())
    {
        LOG_CAMPBELL() << "Json data is an object";
        sensorData = getJsonArray(jsonDoc.object(), "Sensors");
    }
    else
    {
        LOG_CAMPBELL() << "Json Failure";
        return;
    }

    emit clear();
    clearList();
    parseSensors();
}

void SensorManager::clearList()
{
    model->setStringList(QStringList());
}

void SensorManager::on_addevent_clicked()
{
    LOG_CAMPBELL() << "add event clicked";
    QString name = orignals->currentIndex().data(Qt::DisplayRole).toString();

    //If the string exists;
    if (name.compare("") != 0)
    {
        emit addEvent(name);
    }
}

//Parses json to find what sensors are used
void SensorManager::parseSensors()
{
    //setup
    QStringList list;
    QJsonObject value;
    QString name;

    //Get the sensor duration
    if (sensorData.first().isObject())
    {
        QJsonArray data;
        data = getJsonArray(sensorData.at(0).toObject(), "DataArray");
        sensorStartTime = data.first().toObject().find("Timestamp").value().toDouble();
        double end = data.last().toObject().find("Timestamp").value().toDouble();
        sensorTime = (end - sensorStartTime)/1000;

        emit sendSensorDuration(sensorTime);

        LOG_CAMPBELL() << sensorTime;
        LOG_CAMPBELL() << videoTime;

        if (videoTime > 0)
        {
            double difference = fabs(videoTime - sensorTime);
            LOG_CAMPBELL() << QString::number(difference);
            if (difference > 10)
            {
                openDialog(difference);
                LOG_CAMPBELL() << "Error difference is greater than 10 seconds";
            }
        }
    }

    //loop through the sensor data for each item
    for (int i = 0; i < sensorData.size(); i++)
    {
        //Make sure we have the right data type
        if (sensorData.at(i).isObject())
        {
            //get the object
            value = sensorData.at(i).toObject();
            name = value.find("Name:").value().toString();

            //Makes sure each name is unique
            QString concatenate = name;
            int i = 1;
            while (list.contains(concatenate))
            {
                concatenate = name + QString::number(i);
                i++;
            }


            //make sure we have the data
            QJsonArray graphData = getJsonArray(value, "DataArray");
            if (graphData.size() <= 0)
            {
                graphData = getJsonArray(value, "Sensors");
            }

            //If their is still no data we dont set it up
            if (graphData.size() > 0)
            {
                list << concatenate;
                QJsonArray events = value.find("Events").value().toArray();
                std::vector<double> eventData;
                for (int j = 0; j < events.size(); j++)
                {
                    eventData.push_back((events.at(j).toDouble() - sensorStartTime) / 1000);
                }


                //make sure it isnt part of the blacklist
                if (checkBlacklist(name) == false)
                {
                    LOG_CAMPBELL() << "create sensor: " + concatenate;
                    emit sendEvents(concatenate, eventData);
                    emit createGraphDoc(concatenate, graphData);
                }

            }
            else
            {

                LOG_CAMPBELL() << "Error sensor " + concatenate + " has no data in it";
            }
        }
        else
        {
            list << "error at array spot " << QString::number(i);
        }

    }


    model->setStringList(list);

    orignals->setModel(model);
    orignals->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SensorManager::on_blackList_clicked()
{
    BlackList testing;
    testing.exec();
}


//Emits signal to tell main to make a new dock for the given sensor
//Also outputs the sensors data to the console for debugging
void SensorManager::on_listView_doubleClicked(const QModelIndex& index)
{
    QString name = index.data(Qt::DisplayRole).toString();
    if (checkBlacklist(name) == true)
        return;

    for (int i = 0; i < blackList.size(); i++)
    {
        //if the name already exists do nothing
        if (name.compare(blackList.at(i)) == 0)
        {
            LOG_CAMPBELL() << name + " exists skipping open " + blackList.at(i);
            return;
        }
    }

    LOG_CAMPBELL() << "Selected sensor " << name;
    //Calls a function in main to create a new doc
    LOG_CAMPBELL() << name;
    emit graphOpened(name);
}

void SensorManager::on_events_Clicked(const QModelIndex &index)
{
    if (eventsIndex == index.row())
    {
        LOG_CAMPBELL() << "event magic happens";
        events->insertItem(eventsIndex, QString::number(eventsIndex + 1));
        emit createEvents("events" + QString::number(eventsIndex + 1));
        emit graphOpened("events" + QString::number(virtualIndex + 1));
        eventsIndex++;
    }
    else
    {
        emit graphOpened("events" + index.data(Qt::DisplayRole).toString());
    }

}

void SensorManager::on_virtual_clicked(const QModelIndex &index)
{
    if (virtualIndex == index.row())
    {
        LOG_CAMPBELL() << "virtual magic happens";
        virtualSource->insertItem(virtualIndex, QString::number(virtualIndex + 1));
        emit createVirtual("virtual" + QString::number(virtualIndex + 1));
        emit graphOpened("virtual" + QString::number(virtualIndex + 1));
        virtualIndex++;
    }
    else
    {
        emit graphOpened("virtual" + index.data(Qt::DisplayRole).toString());
    }
}

bool SensorManager::checkBlacklist(QString name)
{
    std::vector<QString> options = ShotcutSettings::singleton().getBlackList();
    for (int i = 0; i < options.size(); i++)
    {
        if (options[i].compare(name) == 0)
        {
            return true;
        }
    }
    return false;
}

void SensorManager::updateEvents(QString name, std::vector<double> events)
{
    LOG_CAMPBELL() << name + " Events receieved";
    QRegularExpression find(name);
    int index = model->stringList().indexOf(find);

    QJsonObject sensor = sensorData.at(index).toObject();

    QJsonArray eventValues;
    for (int i = 0; i < events.size(); i++)
    {
        LOG_CAMPBELL() << "i: " + QString::number(i);
        LOG_CAMPBELL() << "event: " + QString::number(events[i]);
        eventValues.push_back(QJsonValue(events[i] * 1000 + sensorStartTime));
    }
    sensor.insert("Events", eventValues);
    sensorData.replace(index, sensor);
}

void SensorManager::saveEvents()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("*.json"));
    dialog.setAcceptMode(dialog.AcceptSave);
    if (dialog.exec())
    {
        QStringList names = dialog.selectedFiles();
        QJsonDocument doc(sensorData);

        QFile file(names.first());
        file.open(QIODevice::ReadWrite);
        file.resize(0);
        QTextStream stream(&file);
        stream << doc.toJson();
    }
}

void SensorManager::videoLength(double time)
{
    videoTime = time;

    if (sensorTime > 0)
    {

        double difference = fabs(videoTime - sensorTime);
        if (difference > 10)
        {
            openDialog(difference);
            LOG_CAMPBELL() << "Error difference is greater than 10 seconds";
        }
    }
}

void SensorManager::createNew()
{
    events->insertItem(eventsIndex, QString::number(eventsIndex + 1));
    emit createVirtual("events" + QString::number(eventsIndex + 1));
    emit graphOpened("events" + QString::number(virtualIndex + 1));
    eventsIndex++;
}

void SensorManager::on_events_closed(QString eventName)
{

}

void SensorManager::openDialog(double time)
{
    QMessageBox dialog;
    dialog.setText("Error the difference between the sensor length and video length is " + QString::number(time));
    dialog.setStandardButtons(QMessageBox::Close);
    dialog.exec();
}

EventContainer* SensorManager::getContainer()
{
    return &sensorEvents;
}

