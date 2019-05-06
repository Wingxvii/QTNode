#include "calibrate.h"

#include <opencv2/highgui.hpp>

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

Calibrate::Calibrate(){
    button = new QPushButton("Innitiate Calibration");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(startCalibration()));
}

unsigned int Calibrate::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 2;

    switch(portType){
    case PortType::In:
        result = 2;
        break;
    case PortType::Out:
        result = 2;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> Calibrate::outData(PortIndex port){
    switch(port){
    case 0:
        if(cameraMat){
            return cameraMat;
        }
        break;
    case 1:
        if(distanceCoeff){
            return distanceCoeff;
        }
        break;
    }
    return NULL;

}

void Calibrate::setInData(std::shared_ptr<NodeData> data, int location){

    switch(location){
    case 0:
    pointsIn = std::dynamic_pointer_cast<PointsData>(data);

        if(pointsIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
          LOG_CAMPBELL() << "Points data was not found";
        }
    break;
    case 1:
         calibDataIn = std::dynamic_pointer_cast<CalibData>(data);

            if(pointsIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();
                //data was found
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
              LOG_CAMPBELL() << "Calibrate data was not found";
            }
        break;

    }
}

NodeDataType Calibrate::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In){
        if(portIndex == 0){
            return PointsData().type();
        }else{
            return CalibData().type();
        }
    }
    return ImageData().type();
}

NodeValidationState Calibrate::validationState() const
{
    return modelValidationState;
}

QString Calibrate::validationMessage() const
{
    return modelValidationError;
}

void Calibrate::startCalibration(){

    if(pointsIn){

        std::vector<std::vector<cv::Point3f>> worldSpaceCornerPoints(1);

        cameraMat = std::make_shared<ImageData>();
        distanceCoeff = std::make_shared<ImageData>();

        cv::Mat cameraMatData;
        cv::Mat distanceCoeffData;

        for (int i = 0; i < calibDataIn->sizeData().height; i++) {
            for (int j = 0; j < calibDataIn->sizeData().width; j++) {
                worldSpaceCornerPoints[0].push_back(cv::Point3f(j * calibDataIn->lengthData(), i*calibDataIn->lengthData(), 0.0f));
            }
        }

        worldSpaceCornerPoints.resize(pointsIn->data().size(), worldSpaceCornerPoints[0]);

        std::vector<cv::Mat> rVectors, tVectors;
        distanceCoeffData = cv::Mat::zeros(8,1,CV_64F);

        cv::calibrateCamera(worldSpaceCornerPoints, pointsIn->data(), calibDataIn->sizeData(), cameraMatData, distanceCoeffData, rVectors, tVectors);

        cameraMat->_image = cameraMatData;
        distanceCoeff->_image = distanceCoeffData;
        LOG_CAMPBELL() << "Calibrate Sucessful";
    }

}


