#include "resizevideonode.h"

ResizeVideoNode::ResizeVideoNode()
{
    //setup ui
    layout = new QGridLayout;
    window = new QWidget;

    interpolationMethod = new QComboBox();
    interpolationMethodLabel = new QLabel("Interpolation Method: ");
    progressText = new QLabel("Inactive");
    progressBar = new QProgressBar();
    resizeLabelX = new QLabel("Resize Scale X: ");
    resizeLabelY = new QLabel("Resize Scale Y: ");

    resizeScaleX = new QLineEdit();
    resizeScaleY = new QLineEdit();

    interpolationMethod->addItem("Nearest Neighbor");
    interpolationMethod->addItem("Bilinear");
    interpolationMethod->addItem("Bicubic");
    interpolationMethod->addItem("Pixel Area Relation");
    interpolationMethod->addItem("Lanczos");
    interpolationMethod->addItem("Bit Exact Bilinear");

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    doublePos = new QDoubleValidator();

    resizeScaleX->setValidator(doublePos);
    resizeScaleY->setValidator(doublePos);

    connect(interpolationMethod, SIGNAL(activated(int)), this, SLOT(preCheck()));
    connect(resizeScaleX, SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(resizeScaleY, SIGNAL(editingFinished()), this, SLOT(preCheck()));

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(&functWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(multiThreadedUpdate()));

    layout->addWidget(interpolationMethodLabel,2,1);
    layout->addWidget(interpolationMethod,2,2);
    layout->addWidget(resizeLabelX,3,1);
    layout->addWidget(resizeLabelY,4,1);
    layout->addWidget(resizeScaleX,3,2);
    layout->addWidget(resizeScaleY,4,2);
    layout->addWidget(progressBar,5,1,1,2);
    layout->addWidget(progressText,6,1,1,2);

    window->setLayout(layout);
    buildContextWindow();
}

unsigned int ResizeVideoNode::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType ResizeVideoNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> ResizeVideoNode::outData(PortIndex port)
{
    return videoOut;
}

void ResizeVideoNode::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState ResizeVideoNode::validationState() const
{
    return modelValidationState;
}

QString ResizeVideoNode::validationMessage() const
{
    return modelValidationError;
}

QJsonObject ResizeVideoNode::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["isReady"] = isReady;
    dataJson["interpIndex"] = interpIndex;
    dataJson["ResizeScaleX"] = ResizeScaleX;
    dataJson["ResizeScaleY"] = ResizeScaleY;
    return dataJson;
}

void ResizeVideoNode::restore(const QJsonObject & json)
{
    if(json.contains("isReady")){
        isReady = json["isReady"].toBool();
    }
    if(json.contains("interpIndex")){
        interpolationMethod->setCurrentIndex(json["interpIndex"].toInt());
        interpIndex = json["interpIndex"].toInt();
    }
    if(json.contains("ResizeScaleX")){
        resizeScaleX->setText(QString::number(json["ResizeScaleX"].toDouble()));
    }
    if(json.contains("ResizeScaleY")){
        resizeScaleY->setText(QString::number(json["ResizeScaleY"].toDouble()));
    }

    preCheck();
}

cv::Mat multiThreadedResize(const cv::Mat &in){
    cv::Mat output;

    cv::resize(in, output, cv::Size(LinkManager::instance()->getFloatData("PRIVATEsizex"), LinkManager::instance()->getFloatData("PRIVATEsizey")));

    return output;
}


void ResizeVideoNode::processData()
{
    double sizex = videoIn->_video[1].cols * ResizeScaleX;
    double sizey = videoIn->_video[1].rows * ResizeScaleY;

    LinkManager::instance()->sendData((float)sizex, "PRIVATEsizex");
    LinkManager::instance()->sendData((float)sizey, "PRIVATEsizey");

    QList<cv::Mat> images = QList<cv::Mat>::fromVector(QVector<cv::Mat>::fromStdVector(videoIn->_video));
    funct = QtConcurrent::mapped(images, multiThreadedResize);

    functWatcher.setFuture(funct);

    progressBar->setMaximum(functWatcher.progressMaximum());
    progressBar->setValue(0);
}


void ResizeVideoNode::preCheck(){

    if(!resizeScaleX->text().isEmpty()){
        ResizeScaleX = resizeScaleX->text().toDouble();
    }
    if(!resizeScaleY->text().isEmpty()){
        ResizeScaleY = resizeScaleY->text().toDouble();
    }
    if(interpolationMethod->currentIndex() != -1){
        interpIndex = interpolationMethod->currentIndex();
    }

    //use this to check if ports are ready
    if(ResizeScaleX != -1.0 &&ResizeScaleY != -1.0 &&  interpIndex != -1){
        isReady = true;
    }else{
        isReady = false;
    }

    if(videoIn && videoIn->isReady && isReady && active){
        processData();
    }else{
        if(videoOut){videoOut->unready();}

    }
}

void ResizeVideoNode::ShowContextMenu(const QPoint &pos)
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

void ResizeVideoNode::multiThreadedUpdate()
{
    progressText->setText(QString::number(functWatcher.progressValue()) + " Of " + QString::number(functWatcher.progressMaximum()) + " Processing...");
    progressBar->setValue(functWatcher.progressValue());

}

void ResizeVideoNode::multiThreadedFinished()
{
    QFutureIterator<cv::Mat> i(funct);
    std::vector<cv::Mat> imageBuffer;

    while(i.hasNext()){
        imageBuffer.push_back(i.next());

        //makes sure there are only good in our list
        if(imageBuffer.back().empty()){
            imageBuffer.pop_back();
        }
    }

    videoOut->_video = imageBuffer;

    //make sure it's filled
    if(imageBuffer.size() >= 1){
        videoOut->ready();
    }

    progressText->setText("Finished");
    progressBar->setValue(functWatcher.progressMaximum());
    videoOut->ready();
    emit dataUpdated(0);
}
