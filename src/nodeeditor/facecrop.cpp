#include "facecrop.h"

FaceCrop::FaceCrop()
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
    layout->addWidget(progressBar);

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int FaceCrop::nPorts(QtNodes::PortType PortType) const
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

QtNodes::NodeDataType FaceCrop::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex == 1){
        return DetectionBoxesData().type();
    }
    return VideoGraphData().type();

}

std::shared_ptr<QtNodes::NodeData> FaceCrop::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return videoOut;
        break;

    }

}

void FaceCrop::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState FaceCrop::validationState() const
{
    return modelValidationState;

}

QString FaceCrop::validationMessage() const
{
    return modelValidationError;

}

void FaceCrop::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &FaceCrop::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void FaceCrop::preCheck()
{
    if(videoIn && videoIn->isReady && dataIn && dataIn->isReady && active ){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }

}

void FaceCrop::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;

    for(int counter = 0; counter < videoIn->_video.size(); counter++){
        if(!dataIn->_boxes[counter].empty()){
            temp.push_back(cv::Mat(videoIn->_video[counter].clone(), dataIn->_boxes[counter][0]));
            lastRect = dataIn->_boxes[counter][0];
        }else if (!lastRect.empty()){
            temp.push_back(cv::Mat(videoIn->_video[counter].clone(), lastRect));
        }
    }
    videoOut->_video = temp;

}

void FaceCrop::multiThreadedFinished()
{
   progressBar->setText("Finished");
   videoOut->ready();
   emit dataUpdated(0);


}
void FaceCrop::ShowContextMenu(const QPoint &pos)
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
