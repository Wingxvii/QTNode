#include "displaycascades.h"

DisplayCascades::DisplayCascades()
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

unsigned int DisplayCascades::nPorts(QtNodes::PortType PortType) const
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

QtNodes::NodeDataType DisplayCascades::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex == 1){
        return DetectionBoxesData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> DisplayCascades::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return videoOut;
        break;

    }
}

void DisplayCascades::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState DisplayCascades::validationState() const
{
    return modelValidationState;

}

QString DisplayCascades::validationMessage() const
{
    return modelValidationError;

}

QJsonObject DisplayCascades::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;

}

void DisplayCascades::restore(const QJsonObject &)
{
    preCheck();
}

void DisplayCascades::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &DisplayCascades::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void DisplayCascades::preCheck()
{

    if(videoIn && videoIn->isReady && dataIn && dataIn->isReady && active ){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void DisplayCascades::ShowContextMenu(const QPoint &pos)
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

void DisplayCascades::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;

    int scale = dataIn->scale;
    cv::Mat frame;
    cv::Point center;

    for(int x = 0; x < videoIn->_video.size(); x++){
        frame = videoIn->_video[x];

        for(cv::Rect rect : dataIn->_boxes[x]){
            center.x = cvRound((rect.x + rect.width*0.5)*scale);
            center.y = cvRound((rect.y + rect.height*0.5)*scale);

            cv::circle(frame, center, cvRound((rect.width + rect.height)*0.25*scale), cv::Scalar(255, 0, 0), 3, 8, 0);

        }
    temp.push_back(frame.clone());

    }

    videoOut->_video = temp;
}

void DisplayCascades::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}
