#include "Max.h"
#include <QDoubleValidator>
#include <Logger.h>

Max::Max(GraphStats stats)
    : Filterable(stats)
{
    layout = new QGridLayout();
    this->setLayout(layout);

    active = new QCheckBox("active");
    layout->addWidget(active, 0, 0);


    max = new QLabel("Max");
    maxValue = new QLineEdit();
    maxValue->setMinimumWidth(50);
    maxValue->setValidator(new QDoubleValidator(0, 1, 5));
    maxValue->setText("0.9");
    layout->addWidget(max, 1, 0);
    layout->addWidget(maxValue, 1, 1);

    connect(active, SIGNAL(clicked(bool)), this, SIGNAL(stateChanged(bool)));
    connect(maxValue, SIGNAL(textChanged(QString)), this, SLOT(filterUpdate()));
}

void Max::updateFilter()
{
    points.clear();
    double xPoint1 = INFINITY, xPoint2 = INFINITY;
    double yPoint1 = INFINITY, yPoint2 = INFINITY;
    bool connected = false;
    bool lastState = false;


    for (auto it = original.begin(); it != original.end(); ++it)
    {
        double xTemp = it->first;
        double yTemp = it->second;

        xPoint2 = xPoint1;
        yPoint2 = yPoint1;

        xPoint1 = xTemp;
        yPoint1 = yTemp;
        float percent = maxValue->text().toFloat();
        if (yPoint2 != INFINITY && yPoint1 != INFINITY)
        {
            if ((yPoint1 - stats.yMin) / stats.yRange > percent)
            {
                lastState = connected;
                connected = true;
                points[xPoint1] = yPoint1;
            }
            else
            {
                lastState = connected;
                connected = false;
            }
            if (connected != lastState)
            {
                //find where it ends
                double y = (percent * stats.yRange) + stats.yMin; //good
                double m = (yPoint1 - yPoint2) / (xPoint1 - xPoint2); // bad?
                double b = yPoint2 - (xPoint2 * m);
                double x = (y - b) / m;
                if (x >= xPoint2 && x <= xPoint1)
                {
                    points[x] = y;
                }
                else
                {
                    LOG_CAMPBELL() << "point failed to calculate";
                }

                //add a point below min range to let our graph now not to connect
                //points.second->filtered[x + ((xPoint1 - x)/2)] = yMin - 1;
            }
        }
    }
}

bool Max::getActive()
{
    return active->isChecked();
}
