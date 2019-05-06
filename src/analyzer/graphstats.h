#ifndef GRAPHSTATS_H
#define GRAPHSTATS_H

#include <math.h>

struct GraphStats
{    
    GraphStats()
    {
        yMin = INFINITY;
        xMin = INFINITY;
        yMax = 0;
        xMax = 0;
    }

    void addData(float xTemp, float yTemp)
    {
        if (xTemp < xMin)
        {
            xMin = xTemp;
        }
        if (xTemp > xMax)
        {
            xMax = xTemp;
        }

        if (yTemp < yMin)
        {
            yMin = yTemp;
        }
        if (yTemp > yMax)
        {
            yMax = yTemp;
        }
        yRange = yMax - yMin;
        xRange = xMax - xMin;
    }

    double yMin;
    double yMax;
    double xMin;
    double xMax;
    double yRange;
    double xRange;
};

#endif // GRAPHSTATS_H
