#include "points2rects.h"

Points2Rects::Points2Rects()
{

    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    pointsIn = std::make_shared<PointsData>();
    boxesOut = std::make_shared<DetectionBoxesData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);


    buildContextWindow();

}

unsigned int Points2Rects::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType Points2Rects::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
            return PointsData().type();
    }else{
            return DetectionBoxesData().type();

    }
}

std::shared_ptr<QtNodes::NodeData> Points2Rects::outData(QtNodes::PortIndex port)
{
    if(port == 0){
        return boxesOut;
    }
}

void Points2Rects::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    pointsIn = std::dynamic_pointer_cast<PointsData>(data);
        if(pointsIn){
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

QtNodes::NodeValidationState Points2Rects::validationState() const
{
    return modelValidationState;

}

QString Points2Rects::validationMessage() const
{
    return modelValidationError;
}

void Points2Rects::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &Points2Rects::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void Points2Rects::preCheck()
{
    if(pointsIn && pointsIn->isReady && active){
        processData();
    }else{
        boxesOut->unready();
    }


}

void Points2Rects::ShowContextMenu(const QPoint &pos)
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

void Points2Rects::multiThreadedProcess()
{
    std::vector<std::vector<cv::Rect>> temp;
    std::vector<std::vector<std::string>> tempNames;

    for(int x = 0; x < pointsIn->_pointList.size(); x++){
        std::vector<cv::Rect> tempFrame;
        std::vector<std::string> tempName;
        for(int y = 0; y < pointsIn->_pointList[x].size(); y++){
            cv::Rect tempRect;
            tempRect.x = pointsIn->_pointList[x][y].x;
            tempRect.y = pointsIn->_pointList[x][y].y;
            tempFrame.push_back(tempRect);
            tempName.push_back(pointsIn->_names[x][y]);
        }
        temp.push_back(tempFrame);
        tempNames.push_back(tempName);
    }
    boxesOut->_boxes = temp;
    boxesOut->_names = tempNames;
}

void Points2Rects::multiThreadedFinished()
{
    progressBar->setText("Finished");
    boxesOut->ready();
    emit dataUpdated(0);
}
