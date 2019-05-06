#ifndef MAX_H
#define MAX_H
#include "filterable.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>

class Max : public Filterable
{
    Q_OBJECT
public:
    Max(GraphStats);

    void updateFilter();
    bool getActive();

private:
    QGridLayout* layout;
    QCheckBox* active;
    QLabel* max;
    QLineEdit* maxValue;
};

#endif // MAX_H
