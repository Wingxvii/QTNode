#include "displaytrackedobject.h"

DisplayTrackedObject::DisplayTrackedObject()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //build layout
    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);
    buildContextWindow();
}

unsigned int DisplayTrackedObject::nPorts(QtNodes::PortType PortType) const
{
    unsigned int result = 1;

    switch(PortType){
    case QtNodes::PortType::In:
        result = 2;
        break;
    case QtNodes::PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

QtNodes::NodeDataType DisplayTrackedObject::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex == 1){
        return DetectionBoxesData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> DisplayTrackedObject::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return videoOut;
        break;
    }
}

void DisplayTrackedObject::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{

    switch(location){
    case 0:
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
        break;

    case 1:
        dataIn = std::dynamic_pointer_cast<DetectionBoxesData>(data);
            if(dataIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();

                preCheck();
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
            }
        break;
    }
}

QtNodes::NodeValidationState DisplayTrackedObject::validationState() const
{
    return modelValidationState;

}

QString DisplayTrackedObject::validationMessage() const
{
    return modelValidationError;

}

QJsonObject DisplayTrackedObject::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;

}

void DisplayTrackedObject::restore(const QJsonObject &)
{
    preCheck();
}

void DisplayTrackedObject::processData()
{
    frameHeight = videoIn->_video[0].rows;
    frameWidth = videoIn->_video[0].cols;

    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &DisplayTrackedObject::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void DisplayTrackedObject::preCheck()
{

    if(videoIn && videoIn->isReady && dataIn && dataIn->isReady && active ){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void DisplayTrackedObject::ShowContextMenu(const QPoint &pos)
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

std::string intToString(int number) {


    std::stringstream ss;
    ss << number;
    return ss.str();
}

void DisplayTrackedObject::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;

    int scale = dataIn->scale;
    cv::Mat frame;

    int x = 0;
    int y = 0;

    for(int counter = 0; counter < (videoIn->_video.size()-1); counter++){
        LOG_JOHN() << "New Frame Started:" << counter << " of " << videoIn->_video.size()-1;

        frame = videoIn->_video[counter].clone();

        for(cv::Rect rect : dataIn->_boxes[counter]){

            LOG_JOHN() << "Beginning to draw...";

            x = rect.x;
            y = rect.y;

            circle(frame, cv::Point(x, y), 20, cv::Scalar(0, 255, 0), 2);
            if (y - 25 > 0)
                line(frame, cv::Point(x, y), cv::Point(x, y - 25), cv::Scalar(0, 255, 0), 2);
            else line(frame, cv::Point(x, y), cv::Point(x, 0), cv::Scalar(0, 255, 0), 2);
            if (y + 25 < frameHeight)
                line(frame, cv::Point(x, y), cv::Point(x, y + 25), cv::Scalar(0, 255, 0), 2);
            else line(frame, cv::Point(x, y), cv::Point(x, frameHeight), cv::Scalar(0, 255, 0), 2);
            if (x - 25 > 0)
                line(frame, cv::Point(x, y), cv::Point(x - 25, y), cv::Scalar(0, 255, 0), 2);
            else line(frame, cv::Point(x, y), cv::Point(0, y), cv::Scalar(0, 255, 0), 2);
            if (x + 25 < frameWidth)
                line(frame, cv::Point(x, y), cv::Point(x + 25, y), cv::Scalar(0, 255, 0), 2);
            else line(frame, cv::Point(x, y), cv::Point(frameWidth, y), cv::Scalar(0, 255, 0), 2);

            LOG_JOHN() << "Drew object";

            cv::putText(frame, intToString(x) + "," + intToString(y), cv::Point(x, y + 30), 1, 1, cv::Scalar(0, 255, 0), 2);

            LOG_JOHN() << "Wrote Text";

        }
    temp.push_back(frame.clone());

    }

    videoOut->_video = temp;
}

void DisplayTrackedObject::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}
