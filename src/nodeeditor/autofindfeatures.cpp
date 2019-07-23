#include "autofindfeatures.h"

AutoFindFeatures::AutoFindFeatures()
{
    //inits
    layout = new QGridLayout;
    window = new QWidget;

    displayCacheIndexLabel = new QLabel("Input Image Output Index:");
    displayCacheIndex = new QLineEdit();
    generateImage = new QCheckBox();


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
    pointsOut = std::make_shared<PointData>();
    displayImage = std::make_shared<ImageData>();

    //setup validators
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    doublePos = new QDoubleValidator();

    maxCorners->setValidator(intPos);
    blockSize->setValidator(intPos);
    qualityLevel->setValidator(doublePos);
    minDistance->setValidator(doublePos);

    //init sample variables
    maxCorners->setText("100");
    qualityLevel->setText("0.3");
    minDistance->setText("7");
    blockSize->setText("7");

    //connections
    connect(generateImage, SIGNAL(stateChanged(int)), this, SLOT(preCheck()));
    connect(maxCorners, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(qualityLevel, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(minDistance, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(blockSize, SIGNAL(returnPressed()), this, SLOT(preCheck()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));


    //build layout
    layout->addWidget(generateImage,1,1);
    layout->addWidget(displayCacheIndexLabel,1,2);
    layout->addWidget(displayCacheIndex,1,3);
    layout->addWidget(maxCornersLabel,3,2);
    layout->addWidget(maxCorners,3,3);
    layout->addWidget(qualityLevelLabel,4,2);
    layout->addWidget(qualityLevel,4,3);
    layout->addWidget(minDistanceLabel,5,2);
    layout->addWidget(minDistance,5,3);
    layout->addWidget(blockSizeLabel,6,2);
    layout->addWidget(blockSize,6,3);
    layout->addWidget(progressBar,7,2);

    window->setLayout(layout);

    buildContextWindow();

}


unsigned int AutoFindFeatures::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType AutoFindFeatures::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
            return VideoGraphData().type();

    }else{

            return PointData().type();
    }
}

std::shared_ptr<NodeData> AutoFindFeatures::outData(PortIndex port)
{
        return pointsOut;
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
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &AutoFindFeatures::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void AutoFindFeatures::preCheck()
{
    if(!maxCorners->text().isEmpty()){
        MaxCorners = maxCorners->text().toInt();
    }
    if(!qualityLevel->text().isEmpty()){
        QualityLevel = qualityLevel->text().toDouble();
    }
    if(!minDistance->text().isEmpty()){
        MinDistance = minDistance->text().toDouble();
    }
    if(!blockSize->text().isEmpty()){
        BlockSize = blockSize->text().toInt();
    }

    if(MaxCorners != -1 && QualityLevel != -1 && MinDistance != -1 && BlockSize != -1){
        isReady = true;
    }else{
        isReady = false;
    }

//&& videoIn->_video[1].type() == CV_8U
    if(videoIn && videoIn->isReady && active && isReady  ){
        processData();
    }else{
        pointsOut->unready();
    }
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
    std::vector<cv::Point2f> temp;
    std::vector<QString> namesTemp;

    //fill pointData with colors
    if(colors.size() < MaxCorners){
        cv::RNG rng;
        for (int i = colors.size(); i <= MaxCorners; i++)
        {
            int r = rng.uniform(0, 256);
            int g = rng.uniform(0, 256);
            int b = rng.uniform(0, 256);
            colors.push_back(cv::Scalar(r, g, b));
        }
    }

    cv::Mat convert = videoIn->_video[0];
    //automatically finds features
    cv::cvtColor(convert, convert,(cv::ColorConversionCodes)6);
    goodFeaturesToTrack(convert, temp, MaxCorners, QualityLevel, MinDistance, cv::Mat(), BlockSize, false, 0.04);


    for(int counter = 0; counter < temp.size(); counter++){
        namesTemp.push_back("Found Feature #" + QString::number(counter));
    }

    pointsOut->_pointList = temp;
    pointsOut->_names = namesTemp;

    LOG_JOHN() << "Size:" << temp.size();
    
}

void AutoFindFeatures::multiThreadedFinished()
{
    onGenImage();
    progressBar->setText("Finished");
    pointsOut->ready();
    emit dataUpdated(0);

}


void AutoFindFeatures::onGenImage()
{
    if(!displayCacheIndex->text().isEmpty()){
        DisplayCacheIndex = displayCacheIndex->text();
    }

    if(generateImage->isChecked() && !DisplayCacheIndex.isEmpty()){
        LOG_JOHN() << "Tried To generate";

        cv::Mat temp = videoIn->_video[0].clone();

        for(int counter = 0; counter < pointsOut->_pointList.size(); counter++){
            cv::circle(temp, pointsOut->_pointList[counter], 4, colors[counter], -1);
            cv::putText(temp, pointsOut->_names[counter].toStdString(), pointsOut->_pointList[counter], cv::FONT_HERSHEY_SIMPLEX, 1,colors[counter]);
        }

        displayImage->_image = temp;

        LinkManager::instance()->sendData(displayImage,DisplayCacheIndex);
    }else{
        LOG_JOHN() << "Did not generate";

    }
}
