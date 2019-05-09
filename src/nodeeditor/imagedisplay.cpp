#include "imagedisplay.h"

#include <opencv/highgui.h>

ImageDisplay::ImageDisplay(){
    button = new QPushButton("Open Image");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(showImage()));
}

unsigned int ImageDisplay::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch(portType)
    {
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 0;
        break;
    default:
        break;
    }
    return result;
}

std::shared_ptr<NodeData> ImageDisplay::outData(PortIndex port){
    return _data;
}

void ImageDisplay::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    _data = std::dynamic_pointer_cast<ImageData>(data);
    if(_data){
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
    }
    else
    {
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        LOG_JOHN() << "Data Not Found";
        //data was not found
    }

}

NodeDataType ImageDisplay::dataType(PortType, PortIndex) const
{
    return ImageData().type();

}

void ImageDisplay::showImage(){
    if(_data)
    {
        cv::namedWindow("Display");
        if(!_data->_image.empty()){
            cv::imshow("Display", _data->_image);
            cv::waitKey(0);
        }

    }
}
NodeValidationState ImageDisplay::validationState() const
{
    return modelValidationState;
}

QString ImageDisplay::validationMessage() const
{
    return modelValidationError;
}
