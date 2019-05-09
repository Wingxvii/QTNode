#include "videosourcedatamodel.h"
#include "nodegraphdata.h"
#include <QFileDialog>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>  // Video write

VideoSourceDataModel::VideoSourceDataModel()
{
    button = new QPushButton("load video");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(chooseVideo()));
}

VideoSourceDataModel::~VideoSourceDataModel()
{

}

unsigned int VideoSourceDataModel::nPorts(PortType portType) const
{
    unsigned int result;

    switch (portType)
    {
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;
}

NodeDataType VideoSourceDataModel::dataType(PortType, PortIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> VideoSourceDataModel::outData(PortIndex)
{
    return _data;
}

void VideoSourceDataModel::chooseVideo()
{
    QString fileName = QFileDialog::getOpenFileName(button, tr("Open sensor"), "");
    cv::VideoCapture capture(fileName.toStdString());

    cv::Mat temp;
    std::vector<cv::Mat> frames;
    capture >> temp;

    int start = 1;
    while (!temp.empty())
    {
        LOG_CAMPBELL() << "id: " + QString::number(start);
        frames.push_back(temp.clone());
        capture >> temp;
        start++;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);
    LOG_JOHN() << "Frames per second using video.get(CAP_PROP_FPS) : " + QString::number(fps);

    _data = std::make_shared<VideoGraphData>(frames);
    _data->setFrameRate(fps);
    emit dataUpdated(0);

}

