#ifndef POINTSDATA_H
#define POINTSDATA_H

#include <nodes/NodeDataModel>
#include "opencv2/core/utility.hpp"

using QtNodes::NodeDataType;
using QtNodes::NodeData;



class PointsData : public NodeData
{
public:
    PointsData()
    {
    }

    PointsData(QString words)
    {
        _text = words;
    }

    PointsData(std::vector<std::vector<cv::Point2f>> points)
    {
        _pointList = points;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"List of Checkerboard Points", "List of Checkerboard Points"};
    }

    std::vector<std::vector<cv::Point2f>> data() const {return _pointList;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }
    void setData(std::vector<std::vector<cv::Point2f>> data){_pointList = data;}

    void addData(std::vector<cv::Point2f> data){_pointList.push_back(data);}

    void removeData(int index){_pointList.erase(_pointList.begin()+index);}

    int getSize(){return _pointList.size();}

    std::vector<std::vector<cv::Point2f>> _pointList;

private:
    QString _text;

};


#endif // POINTSDATA_H
