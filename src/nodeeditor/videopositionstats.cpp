#include "videopositionstats.h"



VideoPositionStats::VideoPositionStats()
{
    layout = new QGridLayout;
    window = new QWidget;
    TotalFrames = new QLabel("Total number of frames: ");
    displayPlot = new HeatMapPlot();
    displayPlot->setTitle("Position Heat Map");

    //connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(displayPlot,1,1);
    layout->addWidget(TotalFrames,2,1);

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int VideoPositionStats::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch(portType){
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

QtNodes::NodeDataType VideoPositionStats::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> VideoPositionStats::outData(QtNodes::PortIndex port)
{
    return inBoxes;
}

void VideoPositionStats::setInData(std::shared_ptr<QtNodes::NodeData>data , int location)
{

    switch(location){
    case 0:
    inBoxes = std::dynamic_pointer_cast<DetectionBoxesData>(data);
        if(inBoxes){
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

QtNodes::NodeValidationState VideoPositionStats::validationState() const
{
    return modelValidationState;
}

QString VideoPositionStats::validationMessage() const
{
    return modelValidationError;
}

QJsonObject VideoPositionStats::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;
}

void VideoPositionStats::restore(const QJsonObject &)
{

}

void VideoPositionStats::processData()
{
    QPolygonF samples;

    for(std::vector<cv::Rect> first : inBoxes->_boxes){
        for(cv::Rect second : first){
            samples += QPointF(second.x, second.y);
        }
    }
    displayPlot->setSamples(samples);
}

void VideoPositionStats::preCheck()
{
    if(inBoxes && inBoxes->isReady && active){
        processData();
    }
}

void VideoPositionStats::updateUI()
{
}

void VideoPositionStats::ShowContextMenu(const QPoint &pos)
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

void VideoPositionStats::multiThreadedProcess()
{

}

void VideoPositionStats::multiThreadedFinished()
{

}

