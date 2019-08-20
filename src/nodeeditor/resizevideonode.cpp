#include "resizevideonode.h"

ResizeVideoNode::ResizeVideoNode()
{
    //setup ui
    layout = new QGridLayout;
    window = new QWidget;

    interpolationMethod = new QComboBox();
    interpolationMethodLabel = new QLabel("Interpolation Method: ");
    progressBar = new QLabel("Inactive");
    resizeLabel = new QLabel("Resize Scale: ");
    resizeScale = new QLineEdit();

    resizeCheckLabel = new QLabel("Resize");
    resize = new QCheckBox();
    rotateCheckLabel = new QLabel("Rotate");
    rotate = new QCheckBox();

    angleLabel = new QLabel("Angle: ");
    angle = new QLineEdit();

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
    connect(resize, SIGNAL(stateChanged(int)), this , SLOT(preCheck()));
    connect(rotate, SIGNAL(stateChanged(int)), this , SLOT(preCheck()));


    layout->addWidget(resizeCheckLabel,1,1);
    layout->addWidget(resize,1,2);
    layout->addWidget(rotateCheckLabel,1,3);
    layout->addWidget(rotate,1,4);
    layout->addWidget(interpolationMethodLabel,2,1);
    layout->addWidget(interpolationMethod,2,2);
    layout->addWidget(resizeLabel,3,1);
    layout->addWidget(resizeScale,3,2);
    layout->addWidget(angleLabel,2,3);
    layout->addWidget(angle,2,4);
    layout->addWidget(progressBar,4,1);

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
    dataJson["ResizeScale"] = ResizeScale;
    dataJson["resizeCheck"] = resize->isChecked();
    dataJson["rotateCheck"] = rotate->isChecked();
    dataJson["Angle"] = Angle;
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
    if(json.contains("ResizeScale")){
        resizeScale->setText(QString::number(json["ResizeScale"].toDouble()));
    }
    if(json.contains("resizeCheck")){
        resize->setChecked(json["resizeCheck"].toBool());
    }
    if(json.contains("rotateCheck")){
        rotate->setChecked(json["rotateCheck"].toBool());
    }
    if(json.contains("Angle")){
        angle->setText(QString::number(json["Angle"].toDouble()));
    }


    preCheck();
}

void ResizeVideoNode::processData()
{
        progressBar->setText("Processing...");

        funct = QtConcurrent::run(this, &ResizeVideoNode::multiThreadedProcess);
        functWatcher.setFuture(funct);

}


void ResizeVideoNode::preCheck(){

    Rotate = rotate->isChecked();
    Resize = resize->isChecked();

    if(!angle->text().isEmpty()){
        Angle = angle->text().toDouble();
    }
    if(!resizeScale->text().isEmpty()){
        ResizeScale = resizeScale->text().toDouble();
    }
    if(interpolationMethod->currentIndex() != -1){
        interpIndex = interpolationMethod->currentIndex();
    }

    //use this to check if ports are ready
    if(ResizeScale != -1.0 && interpIndex != -1 && Resize){
        isReady = true;
    }else if(Angle != -1 && Rotate){
        isReady = true;
    }else{
        isReady = false;
    }


    if(videoIn && videoIn->isReady && isReady && active && isReady){
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

void ResizeVideoNode::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;
    std::vector<cv::Mat> temp2;

    if(Resize){
        double sizex = videoIn->_video[1].cols * ResizeScale;
        double sizey = videoIn->_video[1].rows * ResizeScale;

        //iterate
        for(cv::Mat tempFrame : videoIn->_video){
            cv::Mat resized;
            cv::resize(tempFrame, resized, cv::Size(sizex, sizey));

            temp.push_back(resized);
        }
    }else{
        temp = videoIn->_video;
    }
    if(Rotate){
        for(cv::Mat tempFrame : temp){
            cv::Point2f center((tempFrame.cols-1)/2.0, (tempFrame.rows-1)/2.0);
            cv::Mat rot = cv::getRotationMatrix2D(center, Angle, 1.0);
            cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), tempFrame.size(), Angle).boundingRect2f();

            rot.at<double>(0,2) += bbox.width/2.0 - tempFrame.cols/2.0;
            rot.at<double>(1,2) += bbox.height/2.0 - tempFrame.rows/2.0;

            cv::Mat dst;
            cv::warpAffine(tempFrame, dst, rot, bbox.size());

            temp2.push_back(dst);
        }
    }

    if(!Rotate){
        videoOut->_video = temp;
    }else{
        videoOut->_video = temp2;
    }
}

void ResizeVideoNode::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}
