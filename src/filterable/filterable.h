#ifndef FILTERABLE_H
#define FILTERABLE_H

#include <map>
#include "graphstats.h"
#include <QWidget>

class Filterable : public QWidget
{
    Q_OBJECT
public:
    Filterable(GraphStats);

    virtual std::map<float, float> applyFilter(std::map<float, float>);
    virtual void updateFilter() = 0;
    virtual bool getActive() = 0;

    virtual std::map<float, float> getData();

public slots:
    void filterUpdate();

signals:
    void stateChanged(bool);
    void stateRemoved();

protected:
    std::map<float, float> points;
    std::map<float, float> original;
    GraphStats stats;

};

#endif // FILTERABLE_H
