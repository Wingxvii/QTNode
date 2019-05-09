#include "dilateimage.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"


DilateImage::DilateImage(){
    window = new QWidget;

    button = new QPushButton("Dilate");
    layout = new QFormLayout;

    shapeSelection = new QComboBox();
    shapeLabel = new QLabel("Select Shape");

    shapeSelection->addItem("Rectangle");
    shapeSelection->addItem("Cross");
    shapeSelection->addItem("Ellipse");
    shapeSelection->setCurrentIndex(0);

    sizeSelecton = new QComboBox();
    sizeLabel = new QLabel("Select Size");

    sizeSelecton->addItem("1 Pixel");
    sizeSelecton->addItem("2 Pixel");
    sizeSelecton->addItem("3 Pixel");
    sizeSelecton->addItem("4 Pixel");
    sizeSelecton->addItem("5 Pixel");
    sizeSelecton->addItem("6 Pixel");
    sizeSelecton->addItem("7 Pixel");
    sizeSelecton->addItem("8 Pixel");
    sizeSelecton->addItem("9 Pixel");
    sizeSelecton->addItem("10 Pixel");
    sizeSelecton->setCurrentIndex(7);

    connect(shapeSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(changeShape()));
    connect(sizeSelecton, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSize()));

    layout->addRow(shapeLabel,shapeSelection);
    layout->addRow(sizeLabel, sizeSelecton);
    layout->addRow(button);
    window->setLayout(layout);


    connect(button, SIGNAL(clicked(bool)), this, SLOT(startErosion()));
    videoOut = std::make_shared<VideoGraphData>();
}

unsigned int DilateImage::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> DilateImage::outData(PortIndex port){
    return videoOut;
}

void DilateImage::setInData(std::shared_ptr<NodeData> data, int location){
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
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
NodeDataType DilateImage::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

NodeValidationState DilateImage::validationState() const
{
    return modelValidationState;
}

QString DilateImage::validationMessage() const
{
    return modelValidationError;
}


void DilateImage::changeShape(){

    switch(shapeSelection->currentIndex()){
    case 0:
        shape = cv::MorphShapes::MORPH_RECT;
        break;
    case 1:
        shape = cv::MorphShapes::MORPH_CROSS;
        break;
    case 2:
        shape = cv::MorphShapes::MORPH_ELLIPSE;
        break;
    }

}

void DilateImage::changeSize(){
    size = 1 + sizeSelecton->currentIndex();
}


void DilateImage::startDilation(){
    if (videoIn){
        cv::Mat temp;
        cv::Mat element = cv::getStructuringElement(shape, cv::Size(size,size));

        for(int x = 0; x < videoIn->data().size(); x++){
            cv::dilate(videoIn->data().at(x), temp, element);
            videoOut->_video.push_back(temp.clone());
            LOG_CAMPBELL() << "Converted " + QString::number(x);
        }
        LOG_CAMPBELL() << "Converted Successfully";
    }

}
