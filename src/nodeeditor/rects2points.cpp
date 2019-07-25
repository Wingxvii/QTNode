#include "rects2points.h"


Rects2Points::Rects2Points()
{

    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    pointsOut = std::make_shared<PointsData>();
    boxesIn = std::make_shared<DetectionBoxesData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);


    buildContextWindow();

}

unsigned int Rects2Points::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType Rects2Points::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
            return DetectionBoxesData().type();
    }else{
            return PointsData().type();

    }
}

std::shared_ptr<QtNodes::NodeData> Rects2Points::outData(QtNodes::PortIndex port)
{
    if(port == 0){
        return pointsOut;
    }
}

void Rects2Points::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    boxesIn = std::dynamic_pointer_cast<DetectionBoxesData>(data);
        if(boxesIn){
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

QtNodes::NodeValidationState Rects2Points::validationState() const
{
    return modelValidationState;

}

QString Rects2Points::validationMessage() const
{
    return modelValidationError;
}

void Rects2Points::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &Rects2Points::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void Rects2Points::preCheck()
{
    if(boxesIn && boxesIn->isReady && active){
        processData();
    }else{
        pointsOut->unready();
    }


}

void Rects2Points::ShowContextMenu(const QPoint &pos)
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

void Rects2Points::multiThreadedProcess()
{
    std::vector<std::vector<cv::Point2f>> temp;
    for(std::vector<cv::Rect> frames : boxesIn->_boxes){
        std::vector<cv::Point2f> tempFrame;
        for(cv::Rect rects : frames){
            cv::Point2f tempPoint;
            tempPoint.x = rects.x;
            tempPoint.y = rects.y;
            tempFrame.push_back(tempPoint);
        }
        temp.push_back(tempFrame);
    }
    pointsOut->_pointList = temp;
}

void Rects2Points::multiThreadedFinished()
{
    progressBar->setText("Finished");
    pointsOut->ready();
    emit dataUpdated(0);
}
