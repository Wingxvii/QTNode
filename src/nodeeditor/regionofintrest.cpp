#include "regionofintrest.h"

RegionOfIntrest::RegionOfIntrest()
{
    //setup ui
    layout = new QGridLayout;
    window = new QWidget;

    topLeftCoords = new QLabel("   Top Left Corner: (");
    botRightCoords = new QLabel("Bottom Left Corner: (");
    topComma = new QLabel(",");
    botComma = new QLabel(",");
    topClose = new QLabel(")");
    botClose = new QLabel(")");
    topLeftX = new QLineEdit();
    topLeftY = new QLineEdit();
    botRightX = new QLineEdit();
    botRightY = new QLineEdit();

    progressBar = new QLabel("Inactive");

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    //input regulation unsigned int
    QRegExpValidator* intPos = new QRegExpValidator(QRegExp("\\d*"), this);

    topLeftX->setValidator(intPos);
    topLeftY->setValidator(intPos);
    botRightX->setValidator(intPos);
    botRightY->setValidator(intPos);

    //connections
    connect(topLeftX, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(topLeftY, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(botRightX, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(botRightY, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //build layout
    layout->addWidget(topLeftCoords,1,1);
    layout->addWidget(topLeftX,1,2);
    layout->addWidget(topComma,1,3);
    layout->addWidget(topLeftY,1,4);
    layout->addWidget(topClose,1,5);
    layout->addWidget(botRightCoords,2,1);
    layout->addWidget(botRightX,2,2);
    layout->addWidget(botComma,2,3);
    layout->addWidget(botRightY,2,4);
    layout->addWidget(botClose,2,5);
    layout->addWidget(progressBar,3,1,1,5);
    window->setLayout(layout);

    buildContextWindow();
}


unsigned int RegionOfIntrest::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType RegionOfIntrest::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> RegionOfIntrest::outData(PortIndex port)
{
    return videoOut;
}

void RegionOfIntrest::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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
    }
}

QtNodes::NodeValidationState RegionOfIntrest::validationState() const
{
    return modelValidationState;
}

QString RegionOfIntrest::validationMessage() const
{
    return modelValidationError;
}

QJsonObject RegionOfIntrest::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["isReady"] = isReady;
    dataJson["TopLeftX"] = TopLeftX;
    dataJson["TopLeftY"] = TopLeftY;
    dataJson["BotRightX"] = BotRightX;
    dataJson["BotRightY"] = BotRightY;

    return dataJson;
}

void RegionOfIntrest::restore(const QJsonObject & json)
{
    if(json.contains("isReady")){
        isReady = json["isReady"].toBool();
    }
    if(json.contains("TopLeftX")){
        topLeftX->setText(QString::number(json["TopLeftX"].toInt()));
    }
    if(json.contains("TopLeftY")){
        topLeftY->setText(QString::number(json["TopLeftY"].toInt()));
    }
    if(json.contains("BotRightX")){
        botRightX->setText(QString::number(json["BotRightX"].toInt()));
    }
    if(json.contains("BotRightY")){
        botRightY->setText(QString::number(json["BotRightY"].toInt()));
    }
    preCheck();
}

void RegionOfIntrest::processData()
{
    //setup progress bar parameters
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &RegionOfIntrest::multiThreadedProcess);
    functWatcher.setFuture(funct);


}


void RegionOfIntrest::preCheck(){

    if(!topLeftX->text().isEmpty()){
        TopLeftX = topLeftX->text().toInt();
    }
    if(!topLeftY->text().isEmpty()){
        TopLeftY = topLeftY->text().toInt();
    }
    if(!botRightX->text().isEmpty()){
        BotRightX = botRightX->text().toInt();
    }
    if(!botRightY->text().isEmpty()){
        BotRightY = botRightY->text().toInt();
    }

    if(videoIn && videoIn->isReady && TopLeftX > (videoIn->_video[1].cols-1)){
        TopLeftX = videoIn->_video[1].cols -1;
        topLeftX->setText(QString::number(TopLeftX));
    }
    if(videoIn && videoIn->isReady&& TopLeftY > (videoIn->_video[1].rows-1)){
        TopLeftY = videoIn->_video[1].rows -1;
        topLeftY->setText(QString::number(TopLeftY));
    }
    if(videoIn && videoIn->isReady&& BotRightX > (videoIn->_video[1].cols-1)){
        BotRightX = videoIn->_video[1].cols -1;
        botRightX->setText(QString::number(BotRightX));
    }
    if(videoIn && videoIn->isReady&& BotRightY > (videoIn->_video[1].rows-1)){
        BotRightY = videoIn->_video[1].rows -1;
        botRightY->setText(QString::number(BotRightY));
    }

    if(TopLeftX > BotRightX){
        TopLeftX = 0;
        topLeftX->setText(QString::number(TopLeftX));
    }
    if(TopLeftY > BotRightY){
        TopLeftY = 0;
        topLeftY->setText(QString::number(TopLeftX));
    }

    //use this to check if ports are ready
    if(TopLeftX != -1 && TopLeftY != -1 && BotRightX != -1 && BotRightY!= -1){
        isReady = true;
        LOG_JOHN() << "Ports are ready";
    }

    if(videoIn && videoIn->isReady && isReady && active &&isReady){
        processData();
    }else{
        if(videoOut){videoOut->unready();}

    }
}

void RegionOfIntrest::ShowContextMenu(const QPoint &pos)
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

void RegionOfIntrest::multiThreadedProcess()
{

    LOG_JOHN() << "(" << TopLeftX << "," << TopLeftY << ")";
    LOG_JOHN() << "(" << BotRightX << "," << BotRightY << ")";

    std::vector<cv::Mat> temp;
    //iterate
    for(cv::Mat tempFrame : videoIn->_video){
        cv::Mat cropped(tempFrame, cv::Rect(TopLeftX, TopLeftY, BotRightX - TopLeftX, BotRightY - TopLeftY));

        temp.push_back(cropped);
    }
    videoOut->_video = temp;
}

void RegionOfIntrest::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}
