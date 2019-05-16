#include "imagedisplay.h"

#include <opencv/highgui.h>

ImageDisplay::ImageDisplay(){
    window =  new QWidget;
    layout = new QGridLayout;
    button = new QPushButton("Open Image");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(preCheck()));
    layout->addWidget(button);

    window->setLayout(layout);

    buildContextWindow();

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



void ImageDisplay::processData(){
    cv::namedWindow("Display");
    cv::imshow("Display", _data->_image);
    cv::waitKey(0);
}

void ImageDisplay::preCheck()
{
    if(_data&& !_data->_image.empty() && active){
        processData();
    }else{
        LOG_JOHN() << "Sum ting wong";
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

void ImageDisplay::ShowContextMenu(const QPoint &pos)
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
