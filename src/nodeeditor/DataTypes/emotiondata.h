#ifndef EMOTIONDATA_H
#define EMOTIONDATA_H


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

class EmotionData : public NodeData
{
public:

    EmotionData()
    {
    }

    EmotionData(QString words)
    {
        _text = words;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Emotion Values", "Emotion Values"};
    }

    std::vector<std::map<int, double>> data() const {return _valuePercentages;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }


public:
    QString _text;

    std::vector<std::string> _labels;
    std::vector<std::map<int, double>> _valuePercentages;
    std::vector<int> frameNumbers;

};


#endif // EMOTIONDATA_H
