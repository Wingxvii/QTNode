#ifndef NODEGRAPHDATA_H
#define NODEGRAPHDATA_H

#include <nodes/NodeDataModel>
#include "Logger.h"

using QtNodes::NodeDataType;
using QtNodes::NodeData;

class NodeGraphData : public NodeData
{
public:
    NodeGraphData()
    {
        _data[0] = 0;
    }

    NodeGraphData(std::map<float, float> data)
    {
        _data = data;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"graph", "Graph"};
    }

    std::map<float, float> data() const {return _data;}

    QString dataAsText() const
    {
       QString text = "data: ";
       for (auto it : _data)
       {
           text += QString::number(it.second) + " ";
       }
       return text;
    }


private:
    std::map<float, float> _data;
};

#endif // NodeGraphData_H
