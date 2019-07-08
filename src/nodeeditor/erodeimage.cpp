#include "erodeimage.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"


ErodeImage::ErodeImage(){

    layout = new QGridLayout;
    window =  new QWidget;

    shapeSelection = new QComboBox();
    shapeLabel = new QLabel("Select Shape");
    progressBar = new QLabel("Inactive");

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
    sizeSelecton->setCurrentIndex(2);

    videoOut = std::make_shared<VideoGraphData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(shapeSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(changeShape()));
    connect(sizeSelecton, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSize()));

    layout->addWidget(shapeLabel,1,1);
    layout->addWidget(shapeSelection,1,2);
    layout->addWidget(sizeLabel,2,1);
    layout->addWidget(sizeSelecton,2,2);
    layout->addWidget(progressBar,3,1,1,2);
    window->setLayout(layout);

    buildContextWindow();
}

unsigned int ErodeImage::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> ErodeImage::outData(PortIndex port){
    return videoOut;
}

void ErodeImage::setInData(std::shared_ptr<NodeData> data, int location){
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Video data was not found";
        }

}
NodeDataType ErodeImage::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

NodeValidationState ErodeImage::validationState() const
{
    return modelValidationState;
}

QString ErodeImage::validationMessage() const
{
    return modelValidationError;
}

QJsonObject ErodeImage::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();
    dataJson["size"] = size;
    dataJson["shape"] = shapeSelection->currentIndex();

    return dataJson;

}

void ErodeImage::restore(const QJsonObject & json)
{
    if(json.contains("size")){
        sizeSelecton->setCurrentIndex(json["size"].toInt()-1);
        changeSize();
    }
    if(json.contains("shape")){
        shapeSelection->setCurrentIndex(json["shape"].toInt());
        changeShape();
    }

    preCheck();
}

void ErodeImage::processData()
{
    progressBar->setText("Processing...");


    funct = QtConcurrent::run(this, &ErodeImage::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void ErodeImage::preCheck()
{
    if(videoIn && videoIn->isReady && active){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }

}

void ErodeImage::ShowContextMenu(const QPoint &pos)
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

void ErodeImage::multiThreadedProcess()
{
    cv::Mat temp;
    cv::Mat element = cv::getStructuringElement(shape, cv::Size(size,size));
    std::vector<cv::Mat> tempVideo = std::vector<cv::Mat>();

    for(int x = 0; x < videoIn->data().size(); x++){
        temp = videoIn->data().at(x).clone();
        cv::erode(temp, temp, element);
        tempVideo.push_back(temp.clone());
    }
    videoOut->_video = tempVideo;
}

void ErodeImage::multiThreadedFinished()
{
    LOG_JOHN() << "Converted Successfully";
    progressBar->setText("Finished");

    videoOut->ready();
    emit dataUpdated(0);

}


void ErodeImage::changeShape(){

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

void ErodeImage::changeSize(){
    size = 1 + sizeSelecton->currentIndex();
}
