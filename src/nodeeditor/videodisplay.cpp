#include "videodisplay.h"

#include <opencv2/highgui.hpp>

VideoDisplay::VideoDisplay()
{
    window = new QWidget;

    framerateEdit = new QLineEdit();
    framerateText = new QLabel("Frame Rate");

    formLayout = new QFormLayout;
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    button = new QPushButton("here we go");

    framerateEdit->setValidator(intPos);


    connect(button, SIGNAL(clicked(bool)), this, SLOT(playVideo()));
    connect(framerateEdit, SIGNAL(textChanged(QString)), this, SLOT(saveFrameRate()));

    formLayout->addRow(button);
    formLayout->addRow(framerateText,framerateEdit);

    window->setLayout(formLayout);

}

unsigned int VideoDisplay::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
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

std::shared_ptr<NodeData> VideoDisplay::outData(PortIndex port)
{
    return _data;
}


void VideoDisplay::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    _data = std::dynamic_pointer_cast<VideoGraphData>(data);
    if (_data)
    {
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
        //data was found
    }
    else
    {
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        LOG_JOHN() << "Data Not Found";
        //data was not found
    }
}

NodeDataType VideoDisplay::dataType(PortType, PortIndex) const
{
    return VideoGraphData().type();
}


void VideoDisplay::playVideo()
{

    if (_data)
    {
        auto frames = _data->data();

        /*
        frameRate = _data->getFrameRate();
        */
        if (frameRate == 0)
        {
            LOG_JOHN() << "error: framerate is zero, automaitcally setting framerate to 30";
            frameRate = 30;
        }

        LOG_JOHN() << frameRate;

        cv::namedWindow("Display");
        //LOG_JOHN() << QString::number(frames.size());

        for (int it = 0; it < frames.size(); it++)
        {
            if(cv::getWindowProperty("Display", cv::WND_PROP_VISIBLE) >= 0){
            cv::imshow("Display", frames[it]);
            cv::waitKey( 1000 / frameRate );
            }
        }
        cv::destroyWindow("Display");
    }
}

NodeValidationState VideoDisplay::validationState() const
{
    return modelValidationState;
}

QString VideoDisplay::validationMessage() const
{
    return modelValidationError;
}

void VideoDisplay::saveFrameRate(){
    if(!framerateEdit->text().isEmpty()){
    frameRate = framerateEdit->text().toInt();
    LOG_JOHN() << "Framerate Recieved: " << framerateEdit->text().toInt();
    }
    else{
        frameRate = 0;
    }
}

