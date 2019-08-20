#include "linkout.h"

//##########################     Calib      ################################

CalibLinkOut::CalibLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);


    buildContextWindow();
    preCheck();
}


unsigned int CalibLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType CalibLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return CalibData().type();
}

std::shared_ptr<QtNodes::NodeData> CalibLinkOut::outData(QtNodes::PortIndex port)
{
      return dataOut;
}

QJsonObject CalibLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void CalibLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void CalibLinkOut::processData()
{
    dataOut = LinkManager::instance()->getCalibData(index);
}

void CalibLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getCalibData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getCalibData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void CalibLinkOut::ShowContextMenu(const QPoint &pos)
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


//##########################     Image      ################################

ImageLinkOut::ImageLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    buildContextWindow();
    preCheck();
}


unsigned int ImageLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType ImageLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return ImageData().type();
}

std::shared_ptr<QtNodes::NodeData> ImageLinkOut::outData(QtNodes::PortIndex port)
{
       return dataOut;
}

QJsonObject ImageLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void ImageLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void ImageLinkOut::processData()
{
    dataOut = LinkManager::instance()->getImageData(index);
}

void ImageLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getImageData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getImageData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void ImageLinkOut::ShowContextMenu(const QPoint &pos)
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

//##########################     Point      ################################

PointLinkOut::PointLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    buildContextWindow();
    preCheck();
}


unsigned int PointLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType PointLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return PointData().type();
}

std::shared_ptr<QtNodes::NodeData> PointLinkOut::outData(QtNodes::PortIndex port)
{
      return dataOut;
}

QJsonObject PointLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void PointLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void PointLinkOut::processData()
{
    dataOut = LinkManager::instance()->getPointData(index);
}

void PointLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getPointData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getPointData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void PointLinkOut::ShowContextMenu(const QPoint &pos)
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

//##########################     Points      ################################

PointsLinkOut::PointsLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);


    buildContextWindow();
    preCheck();
}


unsigned int PointsLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType PointsLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return PointsData().type();
}

std::shared_ptr<QtNodes::NodeData> PointsLinkOut::outData(QtNodes::PortIndex port)
{
       return dataOut;
}

QJsonObject PointsLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void PointsLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void PointsLinkOut::processData()
{
    dataOut = LinkManager::instance()->getPointsData(index);
}

void PointsLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getPointsData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getPointsData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void PointsLinkOut::ShowContextMenu(const QPoint &pos)
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

//##########################     Video      ################################

VideoLinkOut::VideoLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    buildContextWindow();
    preCheck();
}


unsigned int VideoLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType VideoLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> VideoLinkOut::outData(QtNodes::PortIndex port)
{
    return dataOut;
}

QJsonObject VideoLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void VideoLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void VideoLinkOut::processData()
{
    dataOut = LinkManager::instance()->getVideoData(index);
}

void VideoLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getVideoData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getVideoData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void VideoLinkOut::ShowContextMenu(const QPoint &pos)
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


//##########################     Detection Boxes      ################################

DetectionLinkOut::DetectionLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    buildContextWindow();
    preCheck();
}


unsigned int DetectionLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType DetectionLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> DetectionLinkOut::outData(QtNodes::PortIndex port)
{
    return dataOut;
}

QJsonObject DetectionLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void DetectionLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void DetectionLinkOut::processData()
{
    dataOut = LinkManager::instance()->getDetectionData(index);
}

void DetectionLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getDetectionData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getDetectionData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void DetectionLinkOut::ShowContextMenu(const QPoint &pos)
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

//##########################     Emotion Data      ################################

EmotionLinkOut::EmotionLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    buildContextWindow();
    preCheck();
}


unsigned int EmotionLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;

    }
    return result;

}

QtNodes::NodeDataType EmotionLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return EmotionData().type();
}

std::shared_ptr<QtNodes::NodeData> EmotionLinkOut::outData(QtNodes::PortIndex port)
{
    return dataOut;
}

QJsonObject EmotionLinkOut::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["Index"] = index.toUtf8().constData();

    return dataJson;

}

void EmotionLinkOut::restore(const QJsonObject &json)
{
    if(json.contains("Index")){
        QString temp = json["Index"].toString();
        indexInput->setText(temp);
    }

    preCheck();

}

void EmotionLinkOut::processData()
{
    dataOut = LinkManager::instance()->getEmotionData(index);
}

void EmotionLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getEmotionData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    if(this->active && LinkManager::instance()->getEmotionData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void EmotionLinkOut::ShowContextMenu(const QPoint &pos)
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
