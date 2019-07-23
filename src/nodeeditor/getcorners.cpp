#include "getcorners.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

GetCorners::GetCorners(){
    window =  new QWidget;
    layout = new QGridLayout;

    progressText = new QLabel("Inactive");
    progressBar = new QProgressBar();

    successDisplay = new QLabel("0");
    successLabel = new QLabel("Succeeded: ");
    failDisplay = new QLabel("0");
    failLabel = new QLabel("Failures: ");

    layout->addWidget(successLabel,1,1);
    layout->addWidget(successDisplay,1,2);
    layout->addWidget(failLabel,2,1);
    layout->addWidget(failDisplay,2,2);
    layout->addWidget(progressBar,3,1);
    layout->addWidget(progressText,4,1);

    cornersOut = std::make_shared<PointsData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(&functWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(multiThreadedUpdate()));

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int GetCorners::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 1;

    switch(portType){
    case PortType::In:
        result = 2;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> GetCorners::outData(PortIndex port){
    switch(port){
    case 0:
        if(cornersOut){
            return cornersOut;
        }
        break;
    }
    return NULL;

}

void GetCorners::setInData(std::shared_ptr<NodeData> data, int location){
    if(location == 0){
        imagesIn = std::dynamic_pointer_cast<VideoGraphData>(data);

        if(imagesIn ){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Image data was not found";
        }

    }else{
        dataIn = std::dynamic_pointer_cast<CalibData>(data);

        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();

        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Calibration data was not found";
        }

    }
}

NodeDataType GetCorners::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::Out){
        return PointsData().type();
    }
    if(portType == PortType::In && portIndex == 1){
        return CalibData().type();
    }
    return VideoGraphData().type();
}

NodeValidationState GetCorners::validationState() const
{
    return modelValidationState;
}

QString GetCorners::validationMessage() const
{
    return modelValidationError;
}

void GetCorners::multiThreadedFinished()
{
    std::vector<std::vector<cv::Point2f>> pointsBuffer;

    QFutureIterator<std::vector<cv::Point2f>> i(funct);

    while(i.hasNext()){
        pointsBuffer.push_back(i.next());

        //makes sure there are only good in our list
        if(pointsBuffer.back().empty()){
            pointsBuffer.pop_back();
        }
    }

    cornersOut->_pointList = pointsBuffer;

    if(LinkManager::instance()->getIntData("PRIVATEsuccesses") > minimumSuccesses){
        cornersOut->ready();
    }

    progressBar->setValue(functWatcher.progressMaximum());

    progressText->setText("Finished");
    emit dataUpdated(0);

}

void GetCorners::multiThreadedUpdate()
{
    progressText->setText(QString::number(functWatcher.progressValue()) + " Of " + QString::number(functWatcher.progressMaximum()) + " Processing...");
    progressBar->setValue(functWatcher.progressValue());

    successDisplay->setText(QString::number(LinkManager::instance()->getIntData("PRIVATEsuccesses")));
    failDisplay->setText(QString::number(LinkManager::instance()->getIntData("PRIVATEfailures")));

}

std::vector<cv::Point2f> multiThreadedGetCorners(const cv::Mat &in)
{
    std::vector<cv::Point2f> pointBuffer;
    //finds actual corners in each image
    bool found = cv::findChessboardCorners(in, LinkManager::instance()->getCalibData("PRIVATEgetcorners1")->sizeData(), pointBuffer, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);

    //pushes into found list if found
    if (found) {
        LOG_JOHN() << "Corners found";
        LinkManager::instance()->sendData(LinkManager::instance()->getIntData("PRIVATEsuccesses") + 1,"PRIVATEsuccesses");
        return pointBuffer;
    }else{
        LOG_JOHN() << "No Corners found";
        LinkManager::instance()->sendData(LinkManager::instance()->getIntData("PRIVATEfailures") + 1,"PRIVATEfailures");
    }
    return {};
}

void GetCorners::processData(){

    //setup progress bar parameters
    progressText->setText("Processing...");

    cornersOut->data().clear();

    //init some variables in linkmanager instance
    LinkManager::instance()->sendData(0,"PRIVATEsuccesses");
    LinkManager::instance()->sendData(0,"PRIVATEfailures");
    LinkManager::instance()->sendData(dataIn, "PRIVATEgetcorners1");

    QList<cv::Mat> images = QList<cv::Mat>::fromVector(QVector<cv::Mat>::fromStdVector(imagesIn->data()));


    funct = QtConcurrent::mapped(images, multiThreadedGetCorners);
    functWatcher.setFuture(funct);
    progressBar->setMaximum(functWatcher.progressMaximum());
    progressBar->setValue(0);

}


void GetCorners::preCheck(){
    if(imagesIn && imagesIn->isReady && dataIn && dataIn->isReady && imagesIn->data().size() && active){
    processData();
    }else{
        if(cornersOut){cornersOut->unready();}

    }
}

void GetCorners::ShowContextMenu(const QPoint &pos)
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


