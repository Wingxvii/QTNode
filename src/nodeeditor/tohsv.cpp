#include "tohsv.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"


ToHSV::ToHSV(){
    button = new QPushButton("Convert to HSV");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(convertToHsv()));
    videoOut = std::make_shared<VideoGraphData>();
}

unsigned int ToHSV::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> ToHSV::outData(PortIndex port){
    return videoOut;
}

void ToHSV::setInData(std::shared_ptr<NodeData> data, int location){
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Video data was not found";
        }

}
NodeDataType ToHSV::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

NodeValidationState ToHSV::validationState() const
{
    return modelValidationState;
}

QString ToHSV::validationMessage() const
{
    return modelValidationError;
}

void ToHSV::convertToHsv(){
    if (videoIn){
        cv::Mat temp;

        for(int x = 0; x < videoIn->data().size(); x++){
            cv::cvtColor(videoIn->data().at(x), temp, cv::COLOR_BGR2HSV);
            videoOut->_video.push_back(temp.clone());
            LOG_JOHN() << "Converted " + QString::number(x);
        }
        LOG_JOHN() << "Converted Successfully";

    }

}
