#ifndef NODEEDITORDATAEXAMPLE_H
#define NODEEDITORDATAEXAMPLE_H

#include <nodes/NodeDataModel>
#include "Logger.h"
#include <qstring.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "qfiledialog.h"
#include "iostream"

//opencv headers
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/core/utility.hpp"

using QtNodes::NodeDataType;
using QtNodes::NodeData;

class NodeEditorDataExample : public NodeData
{
public:


    //
    NodeEditorDataExample(int data)
    {
        _data = data;
    }

    //this is what the input/output labels of nodes in the node editor will display
    NodeDataType type() const override
    {
        return NodeDataType {"Example", "Example"};
    }

    //This function is used to return the data
    int data() const {return _data;}

    //This can be called to get the data as text
    QString dataAsText() const
    {
        QString text = "Put data conversion to text here";
        return text;
    }

private:
    //this is the data
    int _data;

};

#endif // NODEEDITORDATAEXAMPLE_H
