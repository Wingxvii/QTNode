#include "linegraph.h"
#include <Logger.h>
#include <QPainter>
#include <QPaintEvent>
#include <QJsonObject>
#include <mltcontroller.h>
#include <QApplication>
#include <QTimer>
#include <QScrollBar>
#include <QStyle>
#include <QColor>
#include <QElapsedTimer>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLabel>
#include "dialogs/filterlist.h"
#include "Logger.h"


LineGraph::LineGraph()
{
    timer = new QTimer(this);
    timer->start(250);
}

LineGraph::LineGraph(GraphStats graphStats)
{
    setGraphStats(graphStats);
    timer = new QTimer(this);
    timer->start(250);
}

void LineGraph::setGraphStats(GraphStats graphStats)
{
    stats = graphStats;
}

void LineGraph::clearPixmap()
{
    const int ratio = devicePixelRatio();
    const double l_width = width() * ratio;
    const double l_height = height() * ratio;

    pixmap = QPixmap(l_width, l_height);
    pixmap.fill(Qt::black);
}

void LineGraph::updatePixmap(std::map<float, float> data)
{
    const int ratio = devicePixelRatio();
    const double l_width = width() * ratio;
    const double l_height = height() * ratio;
    QPainter p(&pixmap);
    QBrush brush(Qt::white);
    QPen pen(brush, 2);
    p.setPen(pen);
    p.setBrush(brush);
    p.setRenderHint(QPainter::Antialiasing);

    //fancy code magic
    for (auto & points : data)
    {

        double xPoint1 = INFINITY, xPoint2 = INFINITY;
        double yPoint1 = INFINITY, yPoint2 = INFINITY;

        int count = 0;
        count = 0;
        p.setPen(pen);
        xPoint1 = INFINITY; xPoint2 = INFINITY;
        yPoint1 = INFINITY; yPoint2 = INFINITY;
        for (auto & points : data)
        {
            double xTemp = points.first;
            double yTemp = points.second;

            xPoint2 = xPoint1;
            yPoint2 = yPoint1;

            xPoint1 = (((xTemp - stats.xMin) /  (stats.xRange)) * (l_width));
            yPoint1 = l_height - (((yTemp - stats.yMin) / stats.yRange) * l_height);

            count++;
            if (xPoint2 != INFINITY && yPoint2 != INFINITY)
            {
                p.drawLine(xPoint1, yPoint1, xPoint2, yPoint2);
            }
        }
    }
    p.end();
}

void LineGraph::paintEvent(QPaintEvent *e)
{
    const int ratio = devicePixelRatio();
    const double l_width = width() * ratio;
    const double l_height = height() * ratio;


    QPainter p(this);
    QRect r = e->rect();
    p.setClipRect(r);

    QBrush brush(palette().text().color(), Qt::SolidPattern);
    QPen pen(brush, 2);
    p.setPen(pen);
    p.setBrush(brush);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(0, 0, l_width, l_height, pixmap);
}
