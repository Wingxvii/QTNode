#ifndef DETECTIONBOXESDATA_H
#define DETECTIONBOXESDATA_H

#include <nodes/NodeDataModel>
#include "Logger.h"
#include <qstring.h>


#include <stdio.h>
#include <string.h>
#include <time.h>
#include "qfiledialog.h"
#include "iostream"

#include "opencv2/calib3d.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


using QtNodes::NodeDataType;
using QtNodes::NodeData;

class DetectionBoxesData : public NodeData
{
public:
    DetectionBoxesData()
    {
    }

    DetectionBoxesData(QString words)
    {
        _text = words;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Detection Boxes", "Detection Boxes"};
    }

    std::vector<std::vector<cv::Rect>> data() const {
        return _boxes;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }



public:
    std::vector<std::vector<cv::Rect>> _boxes;
    std::vector<std::vector<float>> _conf;
    std::vector<std::vector<std::string>> _names;
    double scale;

    QString _text;


};



#endif // DETECTIONBOXESDATA_H
