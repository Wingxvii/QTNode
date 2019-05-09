#include "undistort.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

UnDistort::UnDistort(){
    button = new QPushButton("Innitiate Undistort");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(startUnDistort()));

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
        if(imageOut){
            return imageOut;
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
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
              LOG_JOHN() << "Distance Coefficient data was not found";
            }
        break;
    case 2:
         imageIn = std::dynamic_pointer_cast<ImageData>(data);

            if(imageIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();
                //data was found
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
    return ImageData().type();
}

NodeValidationState UnDistort::validationState() const
{
    return modelValidationState;
}

QString UnDistort::validationMessage() const
{
    return modelValidationError;
}

void UnDistort::startUnDistort(){

    if(imageIn && cameraMatIn && distanceCoeffIn){
        imageOut = std::make_shared<ImageData>();
        cv::Mat temp = imageOut->_image;

        cv::undistort(imageIn->data(), temp, cameraMatIn->data(), distanceCoeffIn->data());
        imageOut->_image = temp;
        LOG_JOHN() << "Undistort Sucessful";
    }

}

