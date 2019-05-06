#ifndef LINEGRAPH_H
#define LINEGRAPH_H


//Campbells widget

#include <QWidget>
#include <QJsonArray>
#include <QScrollArea>
#include <events.h>
#include <map>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "filterable/graphstats.h"
#include "dialogs/filterlist.h"


class LineGraph : public QWidget
{
    Q_OBJECT
public:
    explicit LineGraph();
    explicit LineGraph(GraphStats graphStats);
    void setGraphStats(GraphStats graphStats);
    void updatePixmap(std::map<float, float> data);
    void clearPixmap();
    void paintEvent(QPaintEvent *e);

private:

    GraphStats stats;
    QTimer *timer;
    QPixmap pixmap;

};

#endif // LINEGRAPH_H
