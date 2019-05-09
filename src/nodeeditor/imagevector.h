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



class ImageVector : public NodeData
{
public:
    ImageVector()
    {
    }

    ImageVector(QString words)
    {
        _text = words;
    }

    ImageVector(std::vector<cv::Mat> image)
    {
        _image = image;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Image list", "Image list"};
    }

    std::vector<cv::Mat> data() const {return _image;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }

    void setData(std::vector<cv::Mat> data){_image = data;}

private:
    QString _text;
    std::vector<cv::Mat> _image;

};


#endif // IMAGEDATA_H
