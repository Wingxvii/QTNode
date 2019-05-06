#include "videothreshold.h"

#include <opencv2/highgui.hpp>

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"


VideoThreshold::VideoThreshold(){

    window = new QWidget;

    HMinEdit = new QSlider(Qt::Horizontal);
    HMaxEdit = new QSlider(Qt::Horizontal);
    SMinEdit = new QSlider(Qt::Horizontal);
    SMaxEdit = new QSlider(Qt::Horizontal);
    VMinEdit = new QSlider(Qt::Horizontal);
    VMaxEdit = new QSlider(Qt::Horizontal);

    HMinEdit->setMinimum(0);
    HMaxEdit->setMinimum(0);
    SMinEdit->setMinimum(0);
    SMaxEdit->setMinimum(0);
    VMinEdit->setMinimum(0);
    VMaxEdit->setMinimum(0);

    HMinEdit->setMaximum(256);
    HMaxEdit->setMaximum(256);
    SMinEdit->setMaximum(256);
    SMaxEdit->setMaximum(256);
    VMinEdit->setMaximum(256);
    VMaxEdit->setMaximum(256);

    HMaxEdit->setValue(255);
    SMaxEdit->setValue(255);
    VMaxEdit->setValue(255);
    \
    HMinText = new QLabel("H Min");
    HMaxText = new QLabel("H Max");
    SMinText = new QLabel("S Min");
    SMaxText = new QLabel("S Max");
    VMinText = new QLabel("V Min");
    VMaxText = new QLabel("V Max");

    button = new QPushButton("Apply Threshold");

    formLayout = new QFormLayout;


    connect(HMinEdit, SIGNAL(valueChanged(int)), this, SLOT(saveHData()));
    connect(HMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(saveHData()));
    connect(SMinEdit, SIGNAL(valueChanged(int)), this, SLOT(saveSData()));
    connect(SMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(saveSData()));
    connect(VMinEdit, SIGNAL(valueChanged(int)), this, SLOT(saveVData()));
    connect(VMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(saveVData()));
    connect(button, SIGNAL(clicked(bool)), this, SLOT(applyThreshold()));

    formLayout->addRow(HMinText, HMinEdit);
    formLayout->addRow(HMaxText, HMaxEdit);
    formLayout->addRow(SMinText, SMinEdit);
    formLayout->addRow(SMaxText, SMaxEdit);
    formLayout->addRow(VMinText, VMinEdit);
    formLayout->addRow(VMaxText, VMaxEdit);
    formLayout->addRow(button);
    window->setLayout(formLayout);

    outVideo = std::make_shared<VideoGraphData>();

}

unsigned int VideoThreshold::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> VideoThreshold::outData(PortIndex port){
    return outVideo;
}

void VideoThreshold::setInData(std::shared_ptr<NodeData> data, int location){
    inVideo = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(inVideo){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_CAMPBELL() << "Video data was not found";
        }

}
NodeDataType VideoThreshold::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

NodeValidationState VideoThreshold::validationState() const
{
    return modelValidationState;
}

QString VideoThreshold::validationMessage() const
{
    return modelValidationError;
}

void VideoThreshold::applyThreshold(){
    if(inVideo){
        cv::Mat temp;
        for(int x = 0; x < inVideo->data().size(); x++){
            cv::inRange(inVideo->data().at(x), cv::Scalar(HMin, SMin, VMin), cv::Scalar(HMax, SMax, VMax), temp);
            outVideo->_video.push_back(temp.clone());
            LOG_CAMPBELL() << "Applied " + QString::number(x);
        }
        LOG_CAMPBELL() << "Applied Successfully";
    }

}

void VideoThreshold::saveHData(){
    LOG_CAMPBELL() << "Saved " + QString::number(HMin) + "," + QString::number(HMax);
    HMin = HMinEdit->value();
    HMax = HMaxEdit->value();

}
void VideoThreshold::saveSData(){
    LOG_CAMPBELL() << "Saved " + QString::number(SMin) + "," + QString::number(SMax);
    SMin = SMinEdit->value();
    SMax = SMaxEdit->value();

}
void VideoThreshold::saveVData(){
    LOG_CAMPBELL() << "Saved " + QString::number(VMin) + "," + QString::number(VMax);
    VMin = VMinEdit->value();
    VMax = VMaxEdit->value();


}
