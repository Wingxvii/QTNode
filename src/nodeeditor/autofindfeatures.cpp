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
    allPoints->setSelectionMode(QAbstractItemView::SingleSelection);

    deleteButton = new QPushButton("Delete");
    addButton = new QPushButton("Add...");
    editButton = new QPushButton("Edit...");
    reGenButton = new QPushButton("Re-Generate Points");
    addInPoints = new QPushButton("Add from Input");
    clearButton = new QPushButton("Clear");

    maxCornersLabel = new QLabel("Max Corners:");
    maxCorners  = new QLineEdit();
    qualityLevelLabel = new QLabel("Quality Level:");
    qualityLevel = new QLineEdit();
    minDistanceLabel = new QLabel("Minimum Distance:");
    minDistance = new QLineEdit();
    blockSizeLabel = new QLabel("Block Size:");
    blockSize = new QLineEdit();

    addWindow = new AddFeature();

    progressBar = new QLabel("Inactive");

    //init ports
    imageOut = std::make_shared<ImageData>();
    pointsOut = std::make_shared<PointData>();

    displayImage = std::make_shared<ImageData>();

    //setup validators
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    doublePos = new QDoubleValidator();

    selectFrame->setValidator(intPos);
    maxCorners->setValidator(intPos);
    blockSize->setValidator(intPos);
    qualityLevel->setValidator(doublePos);
    minDistance->setValidator(doublePos);

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
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    connect(deleteButton, SIGNAL(clicked(bool)), this , SLOT(onDelete()));
    connect(addButton, SIGNAL(clicked(bool)), addWindow , SLOT(openWindow()));
    connect(addWindow, SIGNAL(sendPoint(int, int, QString)), this, SLOT(onAdd(int, int, QString)));
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
    layout->addWidget(allPoints,1,4,5,3);
    layout->addWidget(deleteButton,6,4);
    layout->addWidget(addButton,7,6);
    layout->addWidget(editButton,6,5);
    layout->addWidget(reGenButton,7,4);
    layout->addWidget(addInPoints,7,5);
    layout->addWidget(clearButton,6,6);
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
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &AutoFindFeatures::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void AutoFindFeatures::preCheck()
{

    if(!selectFrame->text().isEmpty()){
        frameSelected = selectFrame->text().toInt();
    }
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

    if(MaxCorners != -1 && QualityLevel != -1 && MinDistance != -1 && BlockSize != -1 && frameSelected != -1){
        if(videoIn && videoIn->isReady && frameSelected >= videoIn->_video.size()){
            frameSelected = videoIn->_video.size()-1;
        }
        isReady = true;
    }else{
        isReady = false;
    }

//&& videoIn->_video[1].type() == CV_8U
    if(videoIn && videoIn->isReady && active && isReady  ){
        processData();
    }else{
        imageOut->unready();
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
    //fill pointData with colors
    if(pointsOut->_colors.size() < MaxCorners){
        cv::RNG rng;
        for (int i = pointsOut->_colors.size(); i <= MaxCorners; i++)
        {
            int r = rng.uniform(0, 256);
            int g = rng.uniform(0, 256);
            int b = rng.uniform(0, 256);
            pointsOut->_colors.push_back(cv::Scalar(r, g, b));
        }
    }

    cv::Mat old_gray = videoIn->_video[frameSelected];
    std::vector<cv::Point2f> p0;
    //automatically finds features
    goodFeaturesToTrack(old_gray, p0, MaxCorners, QualityLevel, MinDistance, cv::Mat(), BlockSize, false, 0.04);

    LOG_JOHN() << "Size:" << p0.size();

    for(cv::Point2f point : p0){
        bool found = false;
        for(cv::Point2f search: pointsOut->_pointList){
            if(point.x == search.x &&point.y == search.y){
                found = true;
            }

        }
        if(!found){
        pointsOut->_pointList.push_back(point);
        pointsOut->_names.push_back("Point #" + QString::number(pointsOut->_names.size()));
        }
    }

}

void AutoFindFeatures::multiThreadedFinished()
{
    allPoints->clear();

    //send points to list
    for(int counter = 0; counter < pointsOut->_pointList.size(); counter++){
        QString tempStr = pointsOut->_names[counter] + " at (" + QString::number(pointsOut->_pointList[counter].x) +
                "," + QString::number(pointsOut->_pointList[counter].y) + ") in Color R:" +
                QString::number(pointsOut->_colors[counter].val[0]) +" G:" +
                QString::number(pointsOut->_colors[counter].val[1]) +" B:" +
                QString::number(pointsOut->_colors[counter].val[2]);

        allPoints->addItem(tempStr);
        LOG_JOHN() << "Sent Point";
    }
    LOG_JOHN() << pointsOut->_pointList.size();

    onGenImage();
    progressBar->setText("Finished");
    imageOut->ready();
    pointsOut->ready();
    emit dataUpdated(0);
    emit dataUpdated(1);

}

void AutoFindFeatures::onGenImage()
{
    if(!displayCacheIndex->text().isEmpty()){
        DisplayCacheIndex = displayCacheIndex->text();
    }

    if(generateImage->isChecked() && !DisplayCacheIndex.isEmpty()){
        LOG_JOHN() << "Tried To generate";

        cv::Mat temp = videoIn->_video[frameSelected].clone();

        for(int counter = 0; counter < pointsOut->_pointList.size(); counter++){
            cv::circle(temp, pointsOut->_pointList[counter], 10, cv::Scalar(0,0,0), -1);
            cv::circle(temp, pointsOut->_pointList[counter], 5, cv::Scalar(255,255,255), -1);
        }

        displayImage->_image = temp;

        LinkManager::instance()->sendData(displayImage,DisplayCacheIndex);
    }else{
        LOG_JOHN() << "Did not generate";

    }
}
void AutoFindFeatures::onDelete(){
    pointsOut->_pointList.erase(pointsOut->_pointList.begin() + allPoints->currentRow());
    pointsOut->_names.erase(pointsOut->_names.begin() + allPoints->currentRow());
    multiThreadedFinished();
}

void AutoFindFeatures::onAdd(int x, int y, QString name)
{
    pointsOut->_pointList.push_back(cv::Point2f(x,y));
    pointsOut->_names.push_back(name);
    multiThreadedFinished();
}

void AutoFindFeatures::onEdit(){

}
void AutoFindFeatures::onRegen(){
    preCheck();
}
void AutoFindFeatures::onClear(){
    pointsOut->_names.clear();
    pointsOut->_pointList.clear();
    multiThreadedFinished();
}
void AutoFindFeatures::onAddInPoints(){

}
