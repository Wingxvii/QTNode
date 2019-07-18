#include "autofindfeatures.h"

AutoFindFeatures::AutoFindFeatures()
{
    //inits
    layout = new QGridLayout;
    window = new QWidget;

    displayCacheIndexLabel = new QLabel("Input Image Output Index:");
    displayCacheIndex = new QLineEdit();
    generateImage = new QCheckBox();

    selectFrameLabel = new QLabel("Select Frame");
    selectFrame = new QLineEdit();

    allPoints= new QListWidget();

    deleteButton = new QPushButton();
    addButton = new QPushButton();
    editButton = new QPushButton();
    reGenButton = new QPushButton();
    addInPoints = new QPushButton();
    clearButton = new QPushButton();

    newCornerLabelX = new QLabel("X");
    newCornerLabelY = new QLabel("Y");
    newCornerX = new QLineEdit();
    newCornerY = new QLineEdit();

    maxCornersLabel = new QLabel("Max Corners:");
    maxCorners  = new QLineEdit();
    qualityLevelLabel = new QLabel("Quality Level:");
    qualityLevel = new QLineEdit();
    minDistanceLabel = new QLabel("Minimum Distance:");
    minDistance = new QLineEdit();
    blockSizeLabel = new QLabel("Block Size:");
    blockSize = new QLineEdit();

    progressBar = new QLabel("Inactive");

    //init ports
    imageOut = std::make_shared<ImageData>();
    pointsOut = std::make_shared<PointData>();

    //setup validators
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    doublePos = new QDoubleValidator();

    selectFrame->setValidator(intPos);
    maxCorners->setValidator(intPos);
    blockSize->setValidator(intPos);
    qualityLevel->setValidator(doublePos);
    minDistance->setValidator(doublePos);
    newCornerX->setValidator(intPos);
    newCornerY->setValidator(intPos);

    //init sample variables
    selectFrame->setText("0");
    maxCorners->setText("100");
    qualityLevel->setText("0.3");
    minDistance->setText("7");
    blockSize->setText("7");

    //connections
    connect(generateImage, SIGNAL(stateChanged(bool)), this, SLOT(preCheck()));
    connect(maxCorners, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(qualityLevel, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(minDistance, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(blockSize, SIGNAL(returnPressed()), this, SLOT(preCheck()));

    connect(deleteButton, SIGNAL(clicked(bool)), this , SLOT(onDelete()));
    connect(addButton, SIGNAL(clicked(bool)), this , SLOT(onAdd()));
    connect(editButton, SIGNAL(clicked(bool)), this , SLOT(onEdit()));
    connect(reGenButton, SIGNAL(clicked(bool)), this , SLOT(onRegen()));
    connect(addInPoints, SIGNAL(clicked(bool)), this , SLOT(onAddInPoints()));
    connect(clearButton, SIGNAL(clicked(bool)), this , SLOT(onClear()));

    //build layout
    layout->addWidget(generateImage,1,1);
    layout->addWidget(displayCacheIndexLabel,1,2);
    layout->addWidget(displayCacheIndex,1,3);
    layout->addWidget(selectFrameLabel,2,2);
    layout->addWidget(selectFrame,2,3);
    layout->addWidget(allPoints,1,5,7,3);
    layout->addWidget(deleteButton,8,5);
    layout->addWidget(addButton,7,4,2,1);
    layout->addWidget(editButton,8,6);
    layout->addWidget(reGenButton,5,4,2,1);
    layout->addWidget(addInPoints,3,4,2,1);
    layout->addWidget(clearButton,8,7);
    layout->addWidget(newCornerLabelX,7,2);
    layout->addWidget(newCornerLabelY,8,2);
    layout->addWidget(newCornerX,7,3);
    layout->addWidget(newCornerY,8,3);
    layout->addWidget(maxCornersLabel,3,2);
    layout->addWidget(maxCorners,3,3);
    layout->addWidget(qualityLevelLabel,4,2);
    layout->addWidget(qualityLevel,4,3);
    layout->addWidget(minDistanceLabel,5,2);
    layout->addWidget(minDistance,5,3);
    layout->addWidget(blockSizeLabel,6,2);
    layout->addWidget(blockSize,6,3);
    layout->addWidget(progressBar,9,2);

    window->setLayout(layout);

    buildContextWindow();
}


unsigned int AutoFindFeatures::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 2;

    switch(portType){
    case PortType::In:
        result = 2;
        break;
    case PortType::Out:
        result = 2;
        break;
    default:
        break;
    }
    return result;
}

QtNodes::NodeDataType AutoFindFeatures::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        if(portIndex == 0){
            return VideoGraphData().type();
        }else{
            return DetectionBoxesData().type();
        }
    }else{
        if(portIndex == 0){
            return ImageData().type();
        }else{
            return PointData().type();
        }
    }
}

std::shared_ptr<NodeData> AutoFindFeatures::outData(PortIndex port)
{
    if(port == 0){
        return imageOut;
    }else{
        return pointsOut;
    }
}

void AutoFindFeatures::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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
    case 1:
    pointsIn = std::dynamic_pointer_cast<DetectionBoxesData>(data);
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

QtNodes::NodeValidationState AutoFindFeatures::validationState() const
{
    return modelValidationState;
}

QString AutoFindFeatures::validationMessage() const
{
    return modelValidationError;
}

void AutoFindFeatures::processData()
{

}

void AutoFindFeatures::preCheck()
{

}

void AutoFindFeatures::ShowContextMenu(const QPoint &pos)
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

void AutoFindFeatures::multiThreadedProcess()
{

}

void AutoFindFeatures::multiThreadedFinished()
{

}

