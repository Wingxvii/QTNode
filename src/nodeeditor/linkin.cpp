#include "linkin.h"


LinkIn::LinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel("Nothing sent");
    indexLabel = new QLabel("Select Data Index: ");
    typeSelectorLabel = new QLabel("Select Data Type: ");
    indexInput = new QLineEdit();
    typeSelection = new QComboBox();

    //input regulation
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    indexInput->setValidator(intPos);

    //setup selections
    typeSelection->addItem("Calibration Data");
    typeSelection->addItem("Image Data");
    typeSelection->addItem("Point Data");
    typeSelection->addItem("Points Data");
    typeSelection->addItem("Video Data");

    //connect
    connect(typeSelection, SIGNAL(currentIndexChanged(int)), this, SLOT(preCheck()));
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(typeSelectorLabel , 2, 1);
    layout->addWidget(typeSelection,2,2);
    layout->addWidget(statusLabel,3,1);
    window->setLayout(layout);

    //ensure this is deactivated
    this->deactivate();

    buildContextWindow();
}

unsigned int LinkIn::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        if(inDataType == dataTypes::NONE){
            result = 0;
        }
        break;
    case PortType::Out:
        result = 0;
        break;
    default:
        break;

    }
    return result;
}

QtNodes::NodeDataType LinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    switch(inDataType){
    case dataTypes::CALIBDATATYPE:
        return CalibData().type();
    case dataTypes::IMAGEDATATYPE:
        return ImageData().type();
    case dataTypes::POINTDATATYPE:
        return PointData().type();
    case dataTypes::POINTSDATATYPE:
        return PointsData().type();
    case dataTypes::VIDEODATATYPE:
        return VideoGraphData().type();
    default:
        return ImageData().type();
    }
}

void LinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    if(data != NULL){
        this->activate();

        dataIn = data;
        preCheck();
    }
}

QtNodes::NodeValidationState LinkIn::validationState() const
{
    return modelValidationState;
}

QString LinkIn::validationMessage() const
{
    return modelValidationError;
}

void LinkIn::processData()
{
    std::shared_ptr<CalibData> calibOut;
    std::shared_ptr<ImageData> imageOut;
    std::shared_ptr<PointData> pointOut;
    std::shared_ptr<PointsData> pointsOut;
    std::shared_ptr<VideoGraphData> videoOut;


    switch(inDataType){
    case dataTypes::CALIBDATATYPE:
        calibOut = std::dynamic_pointer_cast<CalibData>(dataIn);
        LinkManager::instance()->sendData(calibOut ,index);
        break;
    case dataTypes::IMAGEDATATYPE:
        imageOut = std::dynamic_pointer_cast<ImageData>(dataIn);
        LinkManager::instance()->sendData(imageOut ,index);
        break;
    case dataTypes::POINTDATATYPE:
        pointOut = std::dynamic_pointer_cast<PointData>(dataIn);
        LinkManager::instance()->sendData(pointOut ,index);
        break;
    case dataTypes::POINTSDATATYPE:
        pointsOut = std::dynamic_pointer_cast<PointsData>(dataIn);
        LinkManager::instance()->sendData(pointsOut ,index);
        break;
    case dataTypes::VIDEODATATYPE:
        videoOut = std::dynamic_pointer_cast<VideoGraphData>(dataIn);
        LinkManager::instance()->sendData(videoOut ,index);
        break;
    default:
        break;
    }
}

void LinkIn::preCheck()
{
    inDataType = static_cast<dataTypes>(typeSelection->currentIndex());
    index = indexInput->text().toInt();
    if(inDataType != dataTypes::NONE && this->active && dataIn &&dataIn->isReady){
        processData();
    }
}

void LinkIn::ShowContextMenu(const QPoint &pos)
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

