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
        pointList = points;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Checkerboard Points", "Checkerboard Points"};
    }

    std::vector<cv::Point2f> data() const {return pointList;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }

    void setData(std::vector<cv::Point2f> data){pointList = data;}

private:
    QString _text;
    std::vector<cv::Point2f> pointList;

};

#endif // POINTDATA_H
