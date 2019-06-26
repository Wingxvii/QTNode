#ifndef VIDEOGRAPHDATA_H
#define VIDEOGRAPHDATA_H

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



class VideoGraphData : public NodeData
{
public:
    VideoGraphData()
    {
    }

    VideoGraphData(QString words)
    {
        _text = words;
    }

    VideoGraphData(std::vector<cv::Mat> video)
    {
        _video = video;
    }

    void setFrameRate(double framerate)
    {
        _framerate = framerate;
    }

    double getFrameRate()
    {
        return _framerate;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Video", "Video"};
    }

    std::vector<cv::Mat> data() const {
        return _video;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }


    std::vector<cv::Mat> _video;


private:
    QString _text;
    double _framerate = 0;

};

#endif // VIDEOGRAPHDATA_H
