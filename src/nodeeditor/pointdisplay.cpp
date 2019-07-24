#include "pointdisplay.h"

PointDisplay::PointDisplay()
{
    layout = new QGridLayout;
    window =  new QWidget;

    progressBar = new QLabel("Inactive");

    imageOut = std::make_shared<ImageData>();
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    layout->addWidget(progressBar,1,1);
    window->setLayout(layout);

    buildContextWindow();


}

unsigned int PointDisplay::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType PointDisplay::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        if(portIndex == 0){
            return ImageData().type();
        }else{
           return PointData().type();
        }
    }
    return ImageData().type();

}

std::shared_ptr<QtNodes::NodeData> PointDisplay::outData(QtNodes::PortIndex port)
{
return imageOut;
}

void PointDisplay::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{

    switch(location){
    case 0:
    imageIn = std::dynamic_pointer_cast<ImageData>(data);
        if(imageIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    case 1:
        pointIn = std::dynamic_pointer_cast<PointData>(data);
            if(pointIn){
                modelValidationState = NodeValidationState::Valid;
                modelValidationError = QString();

                preCheck();
            }
           else{
              modelValidationState = NodeValidationState::Warning;
              modelValidationError = QStringLiteral("Missing or incorrect inputs");
            }

    }
}

QtNodes::NodeValidationState PointDisplay::validationState() const
{
    return modelValidationState;
}

QString PointDisplay::validationMessage() const
{
    return modelValidationError;
}

void PointDisplay::processData()
{
    //setup progress bar parameters
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &PointDisplay::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void PointDisplay::preCheck()
{
    if(imageIn && imageIn->isReady && pointIn && pointIn->isReady && active){
        processData();
    }else{
        if(imageOut){imageOut->unready();}
    }
}

void PointDisplay::ShowContextMenu(const QPoint &pos)
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

void PointDisplay::multiThreadedProcess()
{
    if(colors.size() < pointIn->_pointList.size()){
        cv::RNG rng;
        for (int i = colors.size(); i <= pointIn->_pointList.size(); i++)
        {
            int r = rng.uniform(0, 256);
            int g = rng.uniform(0, 256);
            int b = rng.uniform(0, 256);
            colors.push_back(cv::Scalar(r, g, b));
        }
    }

    cv::Mat temp = imageIn->_image.clone();

    for(int counter = 0; counter < pointIn->_pointList.size(); counter++){
        cv::circle(temp, pointIn->_pointList[counter], 4, colors[counter], -1);
        cv::putText(temp, pointIn->_names[counter].toStdString(), pointIn->_pointList[counter], cv::FONT_HERSHEY_SIMPLEX, 1,colors[counter]);
    }

    imageOut->_image = temp;
}

void PointDisplay::multiThreadedFinished()
{
    progressBar->setText("Finished");
     imageOut->ready();
     emit dataUpdated(0);

}

