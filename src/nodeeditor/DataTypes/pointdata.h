#ifndef POINTDATA_H
#define POINTDATA_H

#include <nodes/NodeDataModel>
#include "opencv2/core/utility.hpp"


using QtNodes::NodeDataType;
using QtNodes::NodeData;

class PointData : public NodeData
{
public:
    PointData()
    {
    }

    PointData(QString words)
    {
        _text = words;
    }

    PointData(std::vector<cv::Point2f> points)
    {
        _pointList = points;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"List of Points", "List of Points"};
    }

    std::vector<cv::Point2f> data() const {return _pointList;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }

    void setData(std::vector<cv::Point2f> data){_pointList = data;}

public:
    QString _text;
    std::vector<cv::Point2f> _pointList;
    std::vector<QString> _names;
};

#endif // POINTDATA_H
