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
    algorithmSelector = new QComboBox();
    addAlgorithms();


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
    connect(algorithmSelector, SIGNAL(activated(int)), this, SLOT(preCheck()));

    //build layout
    layout->addWidget(generateImage,1,1);
    layout->addWidget(displayCacheIndexLabel,1,2);
    layout->addWidget(displayCacheIndex,1,3);
    layout->addWidget(algorithmSelector,2,2,1,2);
    layout->addWidget(progressBar,3,2);
    /*
    layout->addWidget(maxCornersLabel,3,2);
    layout->addWidget(maxCorners,3,3);
    layout->addWidget(qualityLevelLabel,4,2);
    layout->addWidget(qualityLevel,4,3);
    layout->addWidget(minDistanceLabel,5,2);
    layout->addWidget(minDistance,5,3);
    layout->addWidget(blockSizeLabel,6,2);
    layout->addWidget(blockSize,6,3);
    */
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
        GFTT_MaxCorners = maxCorners->text().toInt();
    }
    if(!qualityLevel->text().isEmpty()){
        GFTT_QualityLevel = qualityLevel->text().toDouble();
    }
    if(!minDistance->text().isEmpty()){
        GFTT_MinDistance = minDistance->text().toDouble();
    }
    if(!blockSize->text().isEmpty()){
        GFTT_BlockSize = blockSize->text().toInt();
    }

    if(GFTT_MaxCorners != -1 && GFTT_QualityLevel != -1 && GFTT_MinDistance != -1 && GFTT_BlockSize != -1){
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



    cv::Mat convert = videoIn->_video[0];
    //automatically finds features

    std::vector<cv::KeyPoint> keypoints;

    if(algorithmSelector->currentIndex() == 0){         //GFTT
        cv::cvtColor(convert, convert,(cv::ColorConversionCodes)6);
        goodFeaturesToTrack(convert, temp, GFTT_MaxCorners, GFTT_QualityLevel, GFTT_MinDistance, cv::Mat(), GFTT_BlockSize, false, 0.04);
    }else if(algorithmSelector->currentIndex() == 1){    //GFTT + Harris
        cv::cvtColor(convert, convert,(cv::ColorConversionCodes)6);
        goodFeaturesToTrack(convert, temp, GFTT_MaxCorners, GFTT_QualityLevel, GFTT_MinDistance, cv::Mat(), GFTT_BlockSize, true, 0.04);
    }else if(algorithmSelector->currentIndex() == 2){   //Agast
        cv::Ptr<cv::AgastFeatureDetector> detector= cv::AgastFeatureDetector::create(AGAST_threshold);
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 3){   //AKAZE
        cv::Ptr<cv::AKAZE> detector= cv::AKAZE::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 4){   //BRISK
        cv::Ptr<cv::BRISK> detector= cv::BRISK::create(BRISK_threshold);
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 5){   //FAST
        cv::Ptr<cv::FastFeatureDetector> detector= cv::FastFeatureDetector::create(FAST_threshold);
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 6){   //KAZE
        cv::Ptr<cv::KAZE> detector= cv::KAZE::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 7){   //MSER
        cv::Ptr<cv::MSER> detector= cv::MSER::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 8){   //ORB
        cv::Ptr<cv::ORB> detector= cv::ORB::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 9){   //Simple Blob
        cv::Ptr<cv::SimpleBlobDetector> detector= cv::SimpleBlobDetector::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 10){  //DAISY
        cv::Ptr<cv::xfeatures2d::DAISY> detector= cv::xfeatures2d::DAISY::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 11){  //FREAK
        cv::Ptr<cv::xfeatures2d::FREAK> detector= cv::xfeatures2d::FREAK::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 12){  //LATCH
        cv::Ptr<cv::xfeatures2d::LATCH> detector= cv::xfeatures2d::LATCH::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 13){  //LUCID
        cv::Ptr<cv::xfeatures2d::LUCID> detector= cv::xfeatures2d::LUCID::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 14){  //MSD
        cv::Ptr<cv::xfeatures2d::MSDDetector> detector= cv::xfeatures2d::MSDDetector::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 15){  //SIFT
        cv::Ptr<cv::xfeatures2d::SIFT> detector= cv::xfeatures2d::SIFT::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 16){  //STAR
        cv::Ptr<cv::xfeatures2d::StarDetector> detector= cv::xfeatures2d::StarDetector::create();
        detector->detect(convert, keypoints);
    }else if(algorithmSelector->currentIndex() == 17){ //SURF
        cv::Ptr<cv::xfeatures2d::SURF> surf= cv::xfeatures2d::SURF::create(SURF_hessianThreshold);
        surf->detect(convert, keypoints);
    }
    else if(algorithmSelector->currentIndex() == 18){   //VGG
        cv::Ptr<cv::xfeatures2d::VGG> detector= cv::xfeatures2d::VGG::create();
        detector->detect(convert, keypoints);
    }


    //turn keypoints into point data
    if(!keypoints.empty()){
        for(int counter = 0; counter < keypoints.size(); counter++){
            temp.push_back(keypoints[counter].pt);
        }
    }

    //fill pointData with colors
    if(colors.size() < temp.size()){
        cv::RNG rng;
        for (int i = colors.size(); i <= temp.size(); i++)
        {
            int r = rng.uniform(0, 256);
            int g = rng.uniform(0, 256);
            int b = rng.uniform(0, 256);
            colors.push_back(cv::Scalar(r, g, b));
        }
    }

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

void AutoFindFeatures::addAlgorithms()
{
    algorithmSelector->addItem("GFTT"); //0
    algorithmSelector->addItem("GFTT + Harris"); //1
    algorithmSelector->addItem("Agast"); //2
    algorithmSelector->addItem("AKAZE"); //3
    algorithmSelector->addItem("BRISK"); //4
    algorithmSelector->addItem("Fast"); //5
    algorithmSelector->addItem("KAZE"); //6
    algorithmSelector->addItem("MSER"); //7
    algorithmSelector->addItem("ORB"); //8
    algorithmSelector->addItem("Simple Blob"); //9
    algorithmSelector->addItem("DAISY"); //10
    algorithmSelector->addItem("FREAK"); //11
    algorithmSelector->addItem("Latch"); //12
    algorithmSelector->addItem("Lucid"); //13
    algorithmSelector->addItem("MSD"); //14
    algorithmSelector->addItem("SIFT"); //15
    algorithmSelector->addItem("StarDetector"); //16
    algorithmSelector->addItem("SURF"); //17
    algorithmSelector->addItem("VGG"); //18

}
