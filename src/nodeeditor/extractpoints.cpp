#include "extractpoints.h"


ExtractPoints::ExtractPoints(){
    layout = new QGridLayout;
    window =  new QWidget;

    totalFramesDisplay = new QLabel(QString::number(totalFrames));
    totalFramesLabel = new QLabel("Total Frames: ");
    forward = new QPushButton("Iterate Forward");
    backward = new QPushButton("Iterate Backward");
    frameTo = new QPushButton("Go To Selected Frame");
    frameNumber = new QLabel("Current Frame: " + QString::number(currFrame));
    frameSelector = new QLineEdit();

    intValid = new QRegExpValidator(QRegExp("\\d*"), this);

    frameSelector->setValidator(intValid);

    connect(forward, SIGNAL(clicked(bool)), this, SLOT(iterateForward()));
    connect(backward, SIGNAL(clicked(bool)), this, SLOT(iterateBackward()));
    connect(frameTo, SIGNAL(clicked(bool)), this, SLOT(gotoFrame()));

    layout->addWidget(totalFramesLabel, 1,1);
    layout->addWidget(totalFramesDisplay, 1,2);
    layout->addWidget(frameNumber,2,1);
    layout->addWidget(frameSelector,3,1);
    layout->addWidget(frameTo,3,2);
    layout->addWidget(forward,4,2);
    layout->addWidget(backward,4,1);
    window->setLayout(layout);

    pointOut = std::make_shared<PointData>();

    buildContextWindow();

}

unsigned int ExtractPoints::nPorts(QtNodes::PortType portType)const
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

std::shared_ptr<NodeData> ExtractPoints::outData(PortIndex port){
    switch(port){
    case 0:
            return pointOut;
        break;
    return NULL;
    }
}

void ExtractPoints::setInData(std::shared_ptr<NodeData> data, int location){

    switch(location){
    case 0:
    pointsIn = std::dynamic_pointer_cast<PointsData>(data);
        if(pointsIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();

            totalFrames = pointsIn->data().size();
            totalFramesDisplay->setText(QString::number(totalFrames));

        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

NodeDataType ExtractPoints::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In){
        return PointsData().type();
    }
    return PointData().type();
}

NodeValidationState ExtractPoints::validationState() const
{
    return modelValidationState;
}

QString ExtractPoints::validationMessage() const
{
    return modelValidationError;
}

void ExtractPoints::iterateForward(){
    ++currFrame;
    preCheck();
}
void ExtractPoints::iterateBackward(){
    --currFrame;
    preCheck();
}

void ExtractPoints::gotoFrame()
{
    currFrame = frameSelector->text().toInt();
    preCheck();
}
void ExtractPoints::processData(){

    pointOut->_pointList = pointsIn->data().at(currFrame);

    std::vector<QString> names;
    for(std::string name : pointsIn->_names.at(currFrame)){
        names.push_back(QString::fromStdString(name));
    }
    if(currFrame != -1){
        pointOut->ready();
    }
}

void ExtractPoints::preCheck()
{
    if(pointsIn){
        //exception handeling for frame size
        if(currFrame > pointsIn->data().size()){
            currFrame = pointsIn->data().size();
        }

        updateUI();
    }

    if(active && pointsIn && pointsIn->isReady){
        processData();
        emit dataUpdated(0);
    }else{
        pointOut->unready();
    }

}

void ExtractPoints::updateUI()
{
    frameNumber->setText("Current Frame: " + QString::number(currFrame));
}

void ExtractPoints::ShowContextMenu(const QPoint &pos)
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

QJsonObject ExtractPoints::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["pointsSelector"] = frameSelector->text().toInt();
    return dataJson;
}

void ExtractPoints::restore(const QJsonObject &json)
{

    if(json.contains("pointsSelector")){
        frameSelector->setText(QString::number(json["pointsSelector"].toInt()));
    }
    preCheck();
}
