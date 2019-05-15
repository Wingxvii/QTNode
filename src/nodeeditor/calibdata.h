#ifndef CALIBDATA_H
#define CALIBDATA_H
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



class CalibData : public NodeData
{
public:
    CalibData()
    {
    }

    CalibData(QString words)
    {
        _text = words;
    }

    CalibData(cv::Size size, float edgeLength)
    {
        boardSize = size;
        squareEdgeLength = edgeLength;
    }

    NodeDataType type() const override
    {
        return NodeDataType {"Checkerboard Information", "Checkerboard Information"};
    }

    cv::Size sizeData() const {return boardSize;}

    float lengthData() const{return squareEdgeLength;}

    QString dataAsText() const
    {
        QString text = "data not representable in text format";
        return text;
    }

    void setSizeXData(int data){boardSize.height = data; xSet = true;}
    void setSizeYData(int data){boardSize.width = data; ySet = true;}
    void setLengthData(float data){squareEdgeLength =  data; lengthSet = true;}


    void checkReady(){
        if(xSet && ySet && lengthSet){
            ready();
        }else{
        unready();
        }
    }

private:
    QString _text;
    cv::Size boardSize;
    float squareEdgeLength;

    bool xSet = false;
    bool ySet = false;
    bool lengthSet = false;

};

#endif // CALIBDATA_H
