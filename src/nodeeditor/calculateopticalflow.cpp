#include "calculateopticalflow.h"

CalculateOpticalFlow::CalculateOpticalFlow()
{
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    videoOut = std::make_shared<VideoGraphData>();
    pointsOut = std::make_shared<PointsData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);


    buildContextWindow();

}

unsigned int CalculateOpticalFlow::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType CalculateOpticalFlow::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        if(portIndex == 0){
            return VideoGraphData().type();
        }else{
            return PointData().type();
        }
    }else{
        if(portIndex == 0){
            return VideoGraphData().type();
        }else{
            return PointsData().type();
        }
    }
}

std::shared_ptr<QtNodes::NodeData> CalculateOpticalFlow::outData(QtNodes::PortIndex port)
{
    if(port == 0){
        return videoOut;
    }else{
        return pointsOut;
    }
}

void CalculateOpticalFlow::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
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
    pointsIn = std::dynamic_pointer_cast<PointData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }

}

QtNodes::NodeValidationState CalculateOpticalFlow::validationState() const
{
    return modelValidationState;

}

QString CalculateOpticalFlow::validationMessage() const
{
    return modelValidationError;
}

void CalculateOpticalFlow::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &CalculateOpticalFlow::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void CalculateOpticalFlow::preCheck()
{
    if(videoIn && videoIn->isReady && pointsIn && pointsIn->isReady && active){
        processData();
    }else{
        videoOut->unready();
        pointsOut->unready();
    }


}

void CalculateOpticalFlow::ShowContextMenu(const QPoint &pos)
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

void CalculateOpticalFlow::multiThreadedProcess()
{

}

void CalculateOpticalFlow::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    pointsOut->ready();
    emit dataUpdated(0);
    emit dataUpdated(1);
}
