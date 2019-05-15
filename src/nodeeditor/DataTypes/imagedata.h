#ifndef IMAGEDATA_H
#define IMAGEDATA_H

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



class ImageData : public NodeData
{
public:

    cv::Mat _image;

    ImageData()
    {
        _image = cv::Mat();
    }

    ImageData(QString words)
    {
        _text = words;
    }

    ImageData(cv::Mat image)
    {
        _image = image;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Single Image", "Single Image"};
    }

    cv::Mat data() const {return _image;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }


private:
    QString _text;

};


#endif // IMAGEDATA_H
