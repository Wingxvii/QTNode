#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QObject>
#include <QScrollArea>
#include <qgridlayout.h>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <events.h>
#include <QJsonArray>
#include <QListView>
#include "filterable/graphstats.h"
#include "analyzer/filternode.h"

class LineGraph;

class GraphData : public QObject
{
    Q_OBJECT

public:
    GraphData(QJsonArray data, QString name, Events* = NULL);
    void setupNodeEditor(FilterNode * node);
    QStringList setupList(QJsonArray data);

    QString getName();
    QGridLayout* getLayout();
    QWidget* getContainer();
    void setDraggable(bool);

    std::map<float, float> getSources();

public slots:
    void updateGraphs();

signals:
    void switched();
    void sendData(QString, std::map<float, float>, GraphStats);

private:
    setupData();

    const int graphIndex = 1;
    Events* events;
    //setup widgets

    struct DataPoints
    {
        std::map<float, float> points;
    };

    LineGraph* lineGraph;
    GraphStats stats;
    std::map<QString, DataPoints> sources;
    DataPoints result;
    DataPoints virtualSource;

    //layout variables
    QWidget* container;
    QScrollArea* scrollarea;
    QGridLayout* layout;
    QTabWidget* tabWidget;
    QPushButton* qButton;
    QLabel* draggable;
    QListView *listview;

    QWidget * listWidget;
    QWidget * graphWidget;
};
#endif // GRAPHDATA_H
