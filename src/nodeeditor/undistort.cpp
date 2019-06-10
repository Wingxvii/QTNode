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

    progressBar = new QLabel("Inactive");

    layout->addWidget(progressBar);

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

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
            return videoOut;
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

void UnDistort::multiThreadedProcess(const cv::Mat &in)
{
    cv::Mat output = cv::Mat();
    cv::undistort(in, output, cameraMatIn->data(), distanceCoeffIn->data());
    temp.push_back(output);

}

void UnDistort::multiThreadedFinished()
{

    videoOut->_video = temp;
    LOG_JOHN() << "Undistort Sucessful";

    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);

}

void UnDistort::processData(){

    //setup progress bar parameters
    progressBar->setText("Processing...");

    videoOut = std::make_shared<VideoGraphData>();

    temp.clear();

    //converts std vector into qvector into qlist
    QList<cv::Mat> videoList = QList<cv::Mat>::fromVector(QVector<cv::Mat>::fromStdVector(videoIn->data()));

    funct = QtConcurrent::map(videoList, &UnDistort::multiThreadedProcess);

    functWatcher.setFuture(funct);


}

void UnDistort::preCheck()
{
    if(videoIn && videoIn->isReady && cameraMatIn && cameraMatIn->isReady && distanceCoeffIn && distanceCoeffIn->isReady && active){
        processData();
    }else{
        if(videoOut) {videoOut->unready();}
    }
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
