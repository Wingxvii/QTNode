#ifndef BASEFILTER_H
#define BASEFILTER_H


#include <map>
#include <vector>

class Filter
{
public:
    Filter();
    Filter(std::map<double, double>);

    void clearFilter();
    void setFilter(std::map<double, double>);
    std::map<double, double> getFilter();
    Filter operator+(const Filter& b);
    //BaseFilter operator-(const BaseFilter& b);


    virtual void filterPoint(double x, double y, double x2, double y2);
    virtual void filterData(std::map<double, double>);

protected:
    std::map<double, double> data;
};

#endif // BASEFILTER_H
