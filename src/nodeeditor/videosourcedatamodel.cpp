#include "videosourcedatamodel.h"
#include "nodegraphdata.h"
#include <QFileDialog>
#include <opencv2/video.hpp>

VideoSourceDataModel::VideoSourceDataModel()
{
    window = new QWidget;
    layout = new QVBoxLayout;

    button = new QPushButton("Choose Video");
    progress = new QProgressBar();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(chooseVideo()));

    layout->addWidget(button);
    layout->addWidget(progress);

    window->setLayout(layout);

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
    QString fileName = QFileDialog::getOpenFileName(button, tr("Choose Video"), "");
    cv::VideoCapture capture(fileName.toStdString());
    progress->setValue(0);

    cv::Mat temp;
    std::vector<cv::Mat> frames;
    capture >> temp;

    int start = 1;

    //setup progress
    progress->setMaximum(capture.get(cv::CAP_PROP_FRAME_COUNT));


    //MULTITHREAD THIS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    while (!temp.empty())
    {
        LOG_CAMPBELL() << "id: " + QString::number(start);

        progress->setValue(start);
        frames.push_back(temp.clone());
        capture >> temp;
        start++;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);

    _data = std::make_shared<VideoGraphData>(frames);
    if(_data->data().size() > 0){
        _data->ready();
    }

    _data->setFrameRate(fps);
    emit dataUpdated(0);

}

