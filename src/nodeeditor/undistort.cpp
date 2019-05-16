#include "undistort.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

UnDistort::UnDistort(){

    window = new QWidget;
    layout = new QGridLayout;

    progressBar = new QProgressBar();

    layout->addWidget(progressBar);

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int UnDistort::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 2;

    switch(portType){
    case PortType::In:
        result = 3;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> UnDistort::outData(PortIndex port){
    switch(port){
    case 0:
        if(videoOut){
            return videoOut;
        }
        break;
    }
    return NULL;
}

void UnDistort::setInData(std::shared_ptr<NodeData> data, int location){

    switch(location){
    case 0:
    cameraMatIn = std::dynamic_pointer_cast<ImageData>(data);

        if(cameraMatIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
          LOG_JOHN() << "Camera Matrix was not found";
        }
    break;
    case 1:
         distanceCoeffIn = std::dynamic_pointer_cast<ImageData>(data);

            if(distanceCoeffIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();
                //data was found
                preCheck();
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
              LOG_JOHN() << "Distance Coefficient data was not found";
            }
        break;
    case 2:
         videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);

            if(videoIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();
                //data was found
                preCheck();
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
              LOG_JOHN() << "Image data was not found";
            }
        break;

    }
}

NodeDataType UnDistort::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex != 2){
        return ImageData().type();
    }
    return VideoGraphData().type();

}

NodeValidationState UnDistort::validationState() const
{
    return modelValidationState;
}

QString UnDistort::validationMessage() const
{
    return modelValidationError;
}

void UnDistort::processData(){

    videoOut = std::make_shared<VideoGraphData>();
    std::vector<cv::Mat> temp;

    for(int i = 0;i < videoIn->data().size(); i++){
        cv::Mat output = cv::Mat();
        cv::undistort(videoIn->data().at(i), output, cameraMatIn->data(), distanceCoeffIn->data());
        temp.push_back(output);
    }

    videoOut->_video = temp;
    LOG_JOHN() << "Undistort Sucessful";

    videoOut->ready();
}

void UnDistort::preCheck()
{
    if(videoIn && videoIn->isReady && cameraMatIn && cameraMatIn->isReady && distanceCoeffIn && distanceCoeffIn->isReady && active){
        processData();
        emit dataUpdated(0);
        updateUI();
    }else{
        if(videoOut) {videoOut->unready();}
    }
}

void UnDistort::updateUI()
{
    progressBar->setValue(100);
}

void UnDistort::ShowContextMenu(const QPoint &pos)
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
