#include "getcorners.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

GetCorners::GetCorners(){

    layout = new QGridLayout;

    successDisplay = new QLabel(QString::number(successes));
    successLabel = new QLabel("Succeeded: ");
    failDisplay = new QLabel(QString::number(failures));
    failLabel = new QLabel("Failures: ");

    layout->addWidget(successLabel,1,1);
    layout->addWidget(successDisplay,1,2);
    layout->addWidget(failLabel,2,1);
    layout->addWidget(failDisplay,2,2);

    cornersOut = std::make_shared<PointsData>();

    window->setLayout(layout);

}

unsigned int GetCorners::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 1;

    switch(portType){
    case PortType::In:
        result = 2;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> GetCorners::outData(PortIndex port){
    switch(port){
    case 0:
        if(cornersOut){
            return cornersOut;
        }
        break;
    }
    return NULL;

}

void GetCorners::setInData(std::shared_ptr<NodeData> data, int location){
    if(location == 0){
        imagesIn = std::dynamic_pointer_cast<VideoGraphData>(data);

        if(imagesIn ){
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

    }else{
        dataIn = std::dynamic_pointer_cast<CalibData>(data);

        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();

        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Calibration data was not found";
        }

    }
}

NodeDataType GetCorners::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::Out){
        return PointsData().type();
    }
    if(portType == PortType::In && portIndex == 1){
        return CalibData().type();
    }
    return VideoGraphData().type();
}

NodeValidationState GetCorners::validationState() const
{
    return modelValidationState;
}

QString GetCorners::validationMessage() const
{
    return modelValidationError;
}

void GetCorners::processData(){

    cornersOut->data().clear();
    std::vector<std::vector<cv::Point2f>> pointsBuffer;

    //these also need to be multithreaded @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    for(int counter = 0; counter < imagesIn->data().size(); counter++){
        std::vector<cv::Point2f> pointBuffer;
        //finds actual corners in each image
        bool found = cv::findChessboardCorners(imagesIn->data().at(counter), dataIn->sizeData(), pointBuffer, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

        //pushes into found list if found
        if (found) {
            pointsBuffer.push_back(pointBuffer);
            LOG_JOHN() << "Corners found";
            ++successes;
        }else{
            LOG_JOHN() << "No Corners found";
            ++failures;
        }
        updateUI();
    }
    //trigger next node
    if(successes > minimumSuccesses){
        cornersOut->pointList = pointsBuffer;
        cornersOut->ready();
    }
}

void GetCorners::preCheck(){
    if(imagesIn && imagesIn->isReady && dataIn && dataIn->isReady && imagesIn->data().size()){
    processData();
    emit dataUpdated(0);
    }else{
        if(cornersOut){cornersOut->unready();}

    }
}

void GetCorners::updateUI(){
    successDisplay->setText(QString::number(successes));
    failDisplay->setText(QString::number(failures));

}

