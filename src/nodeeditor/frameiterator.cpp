#include "frameiterator.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

FrameIterator::FrameIterator(){
    layout = new QGridLayout;

    totalFramesDisplay = new QLabel(QString::number(totalFrames));
    totalFramesLabel = new QLabel("Total Frames: ");
    forward = new QPushButton("Iterate Forward");
    backward = new QPushButton("Iterate Backward");
    frameTo = new QPushButton("Go To Selected Frame");
    frameNumber = new QLabel("Current Frame: " + QString::number(currFrame));
    frameSelector = new QLineEdit();

    intValid = new QRegExpValidator(QRegExp("\\d*"), this);

    frameSelector->setValidator(intValid);

    connect(forward, SIGNAL(clicked(bool)), this, SLOT(iterateForward()));
    connect(backward, SIGNAL(clicked(bool)), this, SLOT(iterateBackward()));
    connect(frameTo, SIGNAL(clicked(bool)), this, SLOT(selectFrame()));

    layout->addWidget(totalFramesLabel, 1,1);
    layout->addWidget(totalFramesDisplay, 1,2);
    layout->addWidget(frameNumber,2,1);
    layout->addWidget(frameSelector,3,1);
    layout->addWidget(frameTo,3,2);
    layout->addWidget(forward,4,2);
    layout->addWidget(backward,4,1);
    window->setLayout(layout);

}

unsigned int FrameIterator::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 1;

    switch(portType){
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> FrameIterator::outData(PortIndex port){
    switch(port){
    case 0:
        if(frameOut){
            return frameOut;
        }
        break;
    return NULL;
    }
}

void FrameIterator::setInData(std::shared_ptr<NodeData> data, int location){

    switch(location){
    case 0:
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
    frameOut = std::make_shared<ImageData>();
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found

            totalFrames = videoIn->data().size();
            totalFramesDisplay->setText(QString::number(totalFrames));

        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
          LOG_JOHN() << "Video data was not found";
        }
    break;
    }
}

NodeDataType FrameIterator::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return ImageData().type();
}

NodeValidationState FrameIterator::validationState() const
{
    return modelValidationState;
}

QString FrameIterator::validationMessage() const
{
    return modelValidationError;
}

void FrameIterator::iterateForward(){

    if(videoIn){

    frameNumber->setText("Current Frame: " + QString::number(++currFrame));
    if(currFrame < videoIn->data().size()){
    frameOut->_image = videoIn->data().at(currFrame);
    }else{
        --currFrame;
    }
    }
}
void FrameIterator::iterateBackward(){
    if(videoIn){
    frameNumber->setText("Current Frame: " + QString::number(--currFrame));
    if(currFrame < videoIn->data().size()){
    frameOut->_image = videoIn->data().at(currFrame);
    }else{
        ++currFrame;
    }
    }
}
void FrameIterator::selectFrame(){
    if(videoIn){
        if(!frameSelector->text().isEmpty()){
            currFrame = frameSelector->text().toInt();
        }
        if(!currFrame < videoIn->data().size()){
            frameNumber->setText("Current Frame: " + QString::number(currFrame));
            frameOut->_image = videoIn->data().at(currFrame);
        }
    }
}

