#include "DebugGetCorners.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

DebugGetCorners::DebugGetCorners(){
    imageOut = std::make_shared<ImageData>();
    cornersOut = std::make_shared<PointData>();

    layout = new QGridLayout;
    window = new QWidget;
    cornersFoundLabel = new QLabel("No Corners Found");

    layout->addWidget(cornersFoundLabel);

    window->setLayout(layout);


}

unsigned int DebugGetCorners::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 2;

    switch(portType){
    case PortType::In:
        result = 2;
        break;
    case PortType::Out:
        result = 2;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> DebugGetCorners::outData(PortIndex port){
    switch(port){
    case 0:
        if(imageOut){
            return imageOut;
        }
        break;
    case 1:
        if(cornersOut){
            return cornersOut;
        }
        break;
    }
    return NULL;

}

void DebugGetCorners::setInData(std::shared_ptr<NodeData> data, int location){
    if(location == 0){
        imageIn = std::dynamic_pointer_cast<ImageData>(data);

        if(imageIn){
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

NodeDataType DebugGetCorners::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::Out && portIndex == 1){
        return PointData().type();
    }
    if(portType == PortType::In && portIndex == 1){
        return CalibData().type();
    }
    return ImageData().type();
}

NodeValidationState DebugGetCorners::validationState() const
{
    return modelValidationState;
}

QString DebugGetCorners::validationMessage() const
{
    return modelValidationError;
}

void DebugGetCorners::processData(){

        std::vector<cv::Point2f> pointBuffer;
        //finds actual corners in each image
        bool found = cv::findChessboardCorners(imageIn->data(), dataIn->sizeData(), pointBuffer, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        imageOut = std::make_shared<ImageData>();

        //pushes into found list if found
        if (found) {
            cornersOut->setData(pointBuffer);
            cornersOut->ready();
            LOG_JOHN() << "Corners found";
            cornersFound = true;
        }else{
            LOG_JOHN() << "No Corners found";
            cornersFound = false;
        }
        //create a copy image of imagein, and send that instead #URGENT
        cv::Mat temp = cv::Mat(imageIn->data().clone());

        cv::drawChessboardCorners(temp, dataIn->sizeData(), pointBuffer, found);
        imageOut->_image = temp;
        imageOut->ready();
        emit dataUpdated(0);
        emit dataUpdated(1);
}

void DebugGetCorners::preCheck()
{
    if(active && imageIn && dataIn && imageIn->isReady && dataIn->isReady){
        processData();
    }else{
        if(cornersOut){cornersOut->unready();updateUI(); cornersFound = false;}
        if(imageOut){imageOut->unready();updateUI();cornersFound = false;}
    }
    updateUI();
}

void DebugGetCorners::updateUI()
{
    if(cornersFound){
        cornersFoundLabel->setText("Corners Found!");

    }else{
        cornersFoundLabel->setText("No Corners Found");

    }

}

void DebugGetCorners::ShowContextMenu(const QPoint &pos)
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


