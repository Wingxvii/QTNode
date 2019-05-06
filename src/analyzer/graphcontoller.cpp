#include "analyzer/graphcontoller.h"
#include <Logger.h>
#include <QJsonObject>
#include <QStringList>
#include <QLabel>
#include <QHBoxLayout>
#include <QListView>
#include "analyzer/linegraph.h"
#include <QScrollArea>
#include <QEvent>
#include <QDockWidget>

GraphController::GraphController(EventContainer *data, QWidget* parent, QGridLayout* graphLayout)
{
    LOG_CAMPBELL() << "Begin";

    setParent(parent);
    this->graphLayout = graphLayout;
    if (data != NULL)
    {
        this->events = data;
    }
    else
    {
        this->events = NULL;
    }

    LOG_CAMPBELL() << "end";
}

void GraphController::setupData(QString name, QJsonArray data)
{
    this->data = data;
    createGraphDataContainer(name, QData);
    this->data = QJsonArray();
}

void GraphController::createEvents(QString name)
{
    createGraphDataContainer(name, eventData, true);
    setupNodeEditor(name, eventData);
    setWidgetDocked(eventData[name]);
}

void GraphController::createVirtual(QString name)
{
    createGraphDataContainer(name, sources, true);
    setupNodeEditor(name, sources);
    setWidgetDocked(sources[name]);
}

void GraphController::setDocked(QString name)
{
    if (QData.find(name) != QData.end())
    {
        setWidgetDocked(QData[name]);
    }
    else if (eventData.find(name) != eventData.end())
    {
        setWidgetDocked(eventData[name]);
    }
    else if (sources.find(name) != sources.end())
    {
        setWidgetDocked(sources[name]);
    }
    else
    {
        LOG_CAMPBELL() << "dock widget does not exists";
    }
}

void GraphController::updateGraphs()
{
    for (auto it = QData.begin(); it != QData.end(); it++)
    {
        it->second->data->updateGraphs();
    }
}

void GraphController::switchWidget(GraphDataContainer* container)
{

    if (container->currentWidget == graphLayout)
    {
        setWidgetDock(container);
    }
    else
    {
        setWidgetDocked(container);
    }

}

void GraphController::setWidgetDock(GraphDataContainer* container)
{
    LOG_CAMPBELL() << "set to dock widget";
    container->currentWidget = container->dock;
    container->dock->setWidget(container->data->getContainer());
    container->dock->show();
    container->data->setDraggable(false);
}

void GraphController::setWidgetDocked(GraphDataContainer* container)
{
    LOG_CAMPBELL() << "set to window widget";
    container->currentWidget = graphLayout;
    graphLayout->addWidget(container->data->getContainer());
    container->dock->hide();
    container->data->setDraggable(true);
}

//creates the container
void GraphController::createGraphDataContainer(QString name, std::map<QString, GraphDataContainer*>& container, bool creation)
{
    if (container.find(name) == container.end())
    {
        GraphData *pointer = new GraphData(data, name, events->getEvents(name, creation));
        GraphDataContainer *gdc = new GraphDataContainer();
        container[name] = gdc;
        container[name]->data = pointer;
        container[name]->dock = new QDockWidget();
        container[name]->currentWidget = graphLayout;

        //lambda function to connect the
        connect(pointer, &GraphData::switched, [&, gdc]()
        {
            switchWidget(gdc);
        });
    }
    else
    {
        LOG_CAMPBELL() << "data already exists";
    }
}

//sets up the node editor
void GraphController::setupNodeEditor(QString name, std::map<QString, GraphDataContainer *> &container)
{
    container[name]->editor = new FilterNode(events->getEvents(name));
    container[name]->data->setupNodeEditor(container[name]->editor);
}
