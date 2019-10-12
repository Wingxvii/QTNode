#include "videosourcedatamodel.h"
#include <QFileDialog>
#include <opencv2/video.hpp>

VideoSourceDataModel::VideoSourceDataModel()
{
    window =  new QWidget;
    layout = new QVBoxLayout;

    button = new QPushButton("Choose Video");
    progressBar = new QLabel("Inactive");

    connect(button, SIGNAL(clicked(bool)), this, SLOT(preCheck()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(button);
    layout->addWidget(progressBar);

    window->setLayout(layout);
    buildContextWindow();

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

std::shared_ptr<NodeData> VideoSourceDataModel::outData(PortIndex port)
{
    return _data;
}

void VideoSourceDataModel::processData()
{
    QString fileName = QFileDialog::getOpenFileName(button, tr("Choose Video"),QString(), tr("Videos (*.mp4 *.avi)"));
    cv::VideoCapture capture(fileName.toStdString());


    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &VideoSourceDataModel::multiThreadedProcess, capture);
    functWatcher.setFuture(funct);

}

void VideoSourceDataModel::preCheck()
{
    if(active){
        processData();
    }
}

void VideoSourceDataModel::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"));

    QAction activateAction("Activate", this);
    QAction deactivateAction("Deactivate", this);

    connect(&activateAction, SIGNAL(triggered()), this, SLOT(activate()));
    connect(&deactivateAction, SIGNAL(triggered()), this, SLOT(deactivate()));
    contextMenu.addAction(&activateAction);
    contextMenu.addAction(&deactivateAction);

    contextMenu.exec(window->mapToGlobal(pos));
}

void VideoSourceDataModel::multiThreadedProcess(cv::VideoCapture capture)
{

    cv::Mat temp;
    std::vector<cv::Mat> frames;
    capture >> temp;
    int start = 1;

    while (!temp.empty())
    {
        LOG_JOHN() << "id: " + QString::number(start);

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

    LOG_JOHN() << "Done";

}

void VideoSourceDataModel::multiThreadedFinished()
{
    LOG_JOHN() << "Emitted Update";
    progressBar->setText("Finished");
    emit dataUpdated(0);
}


