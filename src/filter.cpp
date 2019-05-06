#include "filter.h"
#include <math.h>

Filter::Filter()
{

}

Filter::Filter(std::map<double, double> filter)
{
   setFilter(filter);
}

void Filter::clearFilter()
{
    data.clear();
}

void Filter::setFilter(std::map<double, double> filter)
{
    data = filter;
}

std::map<double, double> Filter::getFilter()
{
    return data;
}

Filter Filter::operator+(const Filter& b)
{
    Filter temp;
    bool finished = false;

    int thisIndex = 0;
    int otherIndex = 0;

    //it->first and it->second
    auto it = data.begin();
    auto kt = b.data.begin();

    double xPoint1 = INFINITY, xPoint2 = INFINITY;
    double yPoint1 = INFINITY, yPoint2 = INFINITY;
    while (it != data.end() || kt != b.data.end())
    {
       // xPoint1 = it
    }

    return temp;
}

void Filter::filterPoint(double x, double y, double x2, double y2)
{
    data[x] = y;
}

void Filter::filterData(std::map<double, double> filter)
{
    data = filter;
}
