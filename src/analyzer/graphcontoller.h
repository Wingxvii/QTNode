//Class made by campbell
//Used to display sensor data

#ifndef GRAPHDOCK_H
#define GRAPHDOCK_H

#include <QDockWidget>
#include <QStringListModel>
#include <QJsonArray>
#include <vector>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <events.h>
#include <QLabel>
#include <QTabWidget>
#include <QPushButton>
#include "analyzer\filternode.h"

#include "analyzer\graphdata.h"

struct GraphDataContainer
{
    //this is the data
    GraphData* data;

    //This is used to filter and create events
    FilterNode* editor;

    //this is used to creater new
    //these are used to control where it is
    QDockWidget* dock;
    QObject* currentWidget;
};

class GraphController : public QObject
{
    Q_OBJECT
public:
    explicit GraphController(EventContainer *container, QWidget* parent, QGridLayout* graphLayout);

    GraphDataContainer* dockedContainer;

public slots:
    //used to create the orignal graphs without filters
    void setupData(QString, QJsonArray);
    void setDocked(QString);
    void createVirtual(QString);
    void createEvents(QString);
    void updateGraphs();

protected:


private:
    void createGraphDataContainer(QString name, std::map<QString, GraphDataContainer*>& container, bool create = false);
    void setupNodeEditor(QString name, std::map<QString, GraphDataContainer*>& container);

    void switchWidget(GraphDataContainer*);
    void setWidgetDock(GraphDataContainer*);
    void setWidgetDocked(GraphDataContainer*);

    bool checkBlacklist(QString);

    QJsonArray data;
    EventContainer *events;
    QGridLayout* graphLayout;

    //this is the widgets of our data
    std::map<QString, GraphDataContainer*> QData;
    std::map<QString, GraphDataContainer*> eventData;
    std::map<QString, GraphDataContainer*> sources;


    int col = 0;
    int row = 0;
};

#endif // GRAPHDOCK_H
