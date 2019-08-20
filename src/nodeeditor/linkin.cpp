#include "linkin.h"

//############################       Calib        #############################

CalibLinkIn::CalibLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    send = new QPushButton("Send Data");
    indexInput->setText("0");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);


    buildContextWindow();
    //preCheck();
}

unsigned int CalibLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType CalibLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return CalibData().type();
}

void CalibLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<CalibData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState CalibLinkIn::validationState() const
{
    return modelValidationState;
}

QString CalibLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject CalibLinkIn::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void CalibLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void CalibLinkIn::processData()
{
    std::shared_ptr<CalibData> calibOut;
    calibOut = std::dynamic_pointer_cast<CalibData>(dataIn);
    LinkManager::instance()->sendData(calibOut ,index);
}

void CalibLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getCalibData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void CalibLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Image        #############################

ImageLinkIn::ImageLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);
    buildContextWindow();
    //preCheck();
}

unsigned int ImageLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType ImageLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ImageData().type();
}

void ImageLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<ImageData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState ImageLinkIn::validationState() const
{
    return modelValidationState;
}

QString ImageLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject ImageLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void ImageLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void ImageLinkIn::processData()
{
    std::shared_ptr<ImageData> imageOut;
    imageOut = std::dynamic_pointer_cast<ImageData>(dataIn);
    LinkManager::instance()->sendData(imageOut ,index);
}

void ImageLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getImageData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void ImageLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Point        #############################


PointLinkIn::PointLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);

    buildContextWindow();
    //preCheck();
}

unsigned int PointLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType PointLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return PointData().type();
}

void PointLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<PointData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState PointLinkIn::validationState() const
{
    return modelValidationState;
}

QString PointLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject PointLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void PointLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void PointLinkIn::processData()
{
    std::shared_ptr<PointData> pointOut;
    pointOut = std::dynamic_pointer_cast<PointData>(dataIn);
    LinkManager::instance()->sendData(pointOut ,index);
}

void PointLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getPointData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void PointLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Points        #############################

PointsLinkIn::PointsLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);

    buildContextWindow();
    //preCheck();
}

unsigned int PointsLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType PointsLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return PointsData().type();
}

void PointsLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<PointsData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState PointsLinkIn::validationState() const
{
    return modelValidationState;
}

QString PointsLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject PointsLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void PointsLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void PointsLinkIn::processData()
{
    std::shared_ptr<PointsData> pointsOut;
    pointsOut = std::dynamic_pointer_cast<PointsData>(dataIn);
    LinkManager::instance()->sendData(pointsOut ,index);
}

void PointsLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getPointsData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void PointsLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Video        #############################


VideoLinkIn::VideoLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);

    buildContextWindow();
    //preCheck();
}

unsigned int VideoLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType VideoLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return VideoGraphData().type();
}

void VideoLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState VideoLinkIn::validationState() const
{
    return modelValidationState;
}

QString VideoLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject VideoLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void VideoLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void VideoLinkIn::processData()
{
    std::shared_ptr<VideoGraphData> videoOut;
    videoOut = std::dynamic_pointer_cast<VideoGraphData>(dataIn);
    LinkManager::instance()->sendData(videoOut ,index);
}

void VideoLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getVideoData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void VideoLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Detection Boxes        #############################


DetectionLinkIn::DetectionLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);

    buildContextWindow();
    //preCheck();
}

unsigned int DetectionLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType DetectionLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return DetectionBoxesData().type();
}

void DetectionLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<DetectionBoxesData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState DetectionLinkIn::validationState() const
{
    return modelValidationState;
}

QString DetectionLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject DetectionLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void DetectionLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void DetectionLinkIn::processData()
{
    std::shared_ptr<DetectionBoxesData> detectionOut;
    detectionOut = std::dynamic_pointer_cast<DetectionBoxesData>(dataIn);
    LinkManager::instance()->sendData(detectionOut ,index);
}

void DetectionLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getVideoData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void DetectionLinkIn::ShowContextMenu(const QPoint &pos)
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

//############################       Emotion Data        #############################


EmotionLinkIn::EmotionLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");
    send = new QPushButton("Send Data");


    //connect
    //connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(send, SIGNAL(clicked(bool)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    layout->addWidget(send, 3,1);
    window->setLayout(layout);

    buildContextWindow();
    //preCheck();
}

unsigned int EmotionLinkIn::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType EmotionLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return EmotionData().type();
}

void EmotionLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<EmotionData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            //preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState EmotionLinkIn::validationState() const
{
    return modelValidationState;
}

QString EmotionLinkIn::validationMessage() const
{
    return modelValidationError;
}

QJsonObject EmotionLinkIn::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;
}

void EmotionLinkIn::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    //preCheck();

}

void EmotionLinkIn::processData()
{
    std::shared_ptr<EmotionData> detectionOut;
    detectionOut = std::dynamic_pointer_cast<EmotionData>(dataIn);
    LinkManager::instance()->sendData(detectionOut ,index);
}

void EmotionLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getVideoData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void EmotionLinkIn::ShowContextMenu(const QPoint &pos)
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

