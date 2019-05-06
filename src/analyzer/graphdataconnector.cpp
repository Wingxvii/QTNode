#include "graphdataconnector.h"
#include "Logger.h"


GraphDataConnector::GraphDataConnector(QObject *parent) : QObject(parent)
{

}


GraphDataConnector& GraphDataConnector::getInstance()
{
    static GraphDataConnector instance;
    return instance;
}

QStringList GraphDataConnector::getNames()
{
    QStringList returnable;
    for (auto names : information)
    {
        returnable << names.first;
    }
    return returnable;
}

std::map<float, float> GraphDataConnector::getData(QString name)
{
    if (information.find(name) != information.end())
    {
        return information[name].data;
    }
    return std::map<float, float>();
}

GraphStats GraphDataConnector::getStats(QString name)
{
    if (information.find(name) != information.end())
    {
        return information[name].stats;
    }
    return GraphStats();
}

void GraphDataConnector::addData(QString name, std::map<float, float> data, GraphStats stats)
{
    information[name].data = data;
    information[name].stats = stats;
    emit updateData();
}

void GraphDataConnector::removeData(QString name)
{
    if (information.find(name) != information.end())
    {
        information.erase(name);
    }
}

GraphDataConnector::~GraphDataConnector()
{

}
