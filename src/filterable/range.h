#ifndef RANGE_H
#define RANGE_H
#include "filterable.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>

class Range : public Filterable
{
    Q_OBJECT
public:
    Range(GraphStats);

    void updateFilter();
    bool getActive();

private:
    QGridLayout* layout;
    QCheckBox* active;
    QLabel* max;
    QLabel* min;
    QLineEdit* maxValue;
    QLineEdit* minValue;
};

#endif // RANGE_H
