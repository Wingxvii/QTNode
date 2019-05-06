#include "filterable.h"

Filterable::Filterable(GraphStats statistics)
{
    stats = statistics;
}

std::map<float, float> Filterable::getData()
{
    return points;
}

void Filterable::filterUpdate()
{
    updateFilter();
}

std::map<float, float> Filterable::applyFilter(std::map<float, float> data)
{
    original = data;
    updateFilter();
    return points;
}
