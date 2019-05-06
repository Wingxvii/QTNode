#ifndef MIN_H
#define MIN_H
#include "filterable.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>

class Min : public Filterable
{
    Q_OBJECT
public:
    Min(GraphStats);

    void updateFilter();
    bool getActive();

private:
    QGridLayout* layout;
    QCheckBox* active;
    QLabel* min;
    QLineEdit* minValue;
};

#endif // MIN_H
