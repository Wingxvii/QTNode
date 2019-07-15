#include "resizevideo.h"

ResizeVideo::ResizeVideo()
{
    //setup ui
    layout = new QGridLayout;
    window = new QWidget;

    interpolationMethod = new QComboBox();
    interpolationMethodLabel = new QLabel("Interpolation Method: ");
    progressBar = new QLabel("Inactive");
    resizeLabel = new QLabel("Resize Scale: ");
    resizeScale = new QLineEdit();

    interpolationMethod->addItem("Nearest Neighbor");
    interpolationMethod->addItem("Bilinear");
    interpolationMethod->addItem("Bicubic");
    interpolationMethod->addItem("Pixel Area Relation");
    interpolationMethod->addItem("Lanczos");
    interpolationMethod->addItem("Bit Exact Bilinear");

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    doublePos = new QDoubleValidator();

    resizeScale->setValidator(doublePos);

    connect(interpolationMethod, SIGNAL(activated(int)), this, SLOT(preCheck()));
    connect(resizeScale, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(interpolationMethodLabel,1,1);
    layout->addWidget(interpolationMethod,1,2);
    layout->addWidget(resizeLabel,2,1);
    layout->addWidget(resizeScale,2,2);
    buildContextWindow();
}

unsigned int ResizeVideo::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType ResizeVideo::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> ResizeVideo::outData(PortIndex port)
{
    return videoOut;
}

void ResizeVideo::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState ResizeVideo::validationState() const
{
    return modelValidationState;
}

QString ResizeVideo::validationMessage() const
{
    return modelValidationError;
}

QJsonObject ResizeVideo::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["isReady"] = isReady;
    dataJson["interpIndex"] = interpIndex;
    dataJson["ResizeScale"] = ResizeScale;

    return dataJson;
}

void ResizeVideo::restore(const QJsonObject & json)
{
    if(json.contains("isReady")){
        isReady = json["isReady"].toBool();
    }
    if(json.contains("interpIndex")){
        interpolationMethod->setCurrentIndex(json["interpIndex"].toInt());
        interpIndex = json["interpIndex"].toInt();
    }
    if(json.contains("ResizeScale")){
        resizeScale->setText(QString::number(json["ResizeScale"].toInt()));
    }

    preCheck();
}

void ResizeVideo::processData()
{
    //setup progress bar parameters
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &ResizeVideo::multiThreadedProcess);
    functWatcher.setFuture(funct);


}


void ResizeVideo::preCheck(){

    if(!resizeScale->text().isEmpty()){
        ResizeScale = resizeScale->text().toInt();
    }
    if(interpolationMethod->currentIndex() != -1){
        interpIndex = interpolationMethod->currentIndex();
    }

    //use this to check if ports are ready
    if(ResizeScale != -1.0 && interpIndex != -1){
        isReady = true;
    }


    if(videoIn && videoIn->isReady && isReady && active && isReady){
        processData();
    }else{
        if(videoOut){videoOut->unready();}

    }
}

void ResizeVideo::ShowContextMenu(const QPoint &pos)
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

void ResizeVideo::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;
    //iterate
    for(cv::Mat tempFrame : videoIn->_video){
        cv::Mat resized;
        cv::resize(tempFrame, resized, cv::Size((int)videoIn->_video[1].cols * ResizeScale, (int)videoIn->_video[1].rows * ResizeScale));

        temp.push_back(resized);
    }
    videoOut->_video = temp;
}

void ResizeVideo::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}
