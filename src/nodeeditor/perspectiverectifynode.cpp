#include "perspectiverectifynode.h"
#include <tgmath.h>
PerspectiveRectify::PerspectiveRectify()
{
    //setup ui
    layout = new QGridLayout;
    window = new QWidget;

    coords1 = new QLabel("Top Left Corner:");
    coords2 = new QLabel("Top Right Corner:");
    coords3 = new QLabel("Bottom Right Corner:");
    coords4 = new QLabel("Bottom Left Corner:");

    comma1 = new QLabel(",");
    comma2 = new QLabel(",");
    comma3 = new QLabel(",");
    comma4 = new QLabel(",");

    close1 = new QLabel(")");
    close2 = new QLabel(")");
    close3 = new QLabel(")");
    close4 = new QLabel(")");

    corner1X = new QLineEdit();
    corner1Y = new QLineEdit();
    corner2X = new QLineEdit();
    corner2Y = new QLineEdit();
    corner3X = new QLineEdit();
    corner3Y = new QLineEdit();
    corner4X = new QLineEdit();
    corner4Y = new QLineEdit();

    progressText = new QLabel("Inactive");
    progressBar = new QProgressBar();

    //init out port
    videoOut = std::make_shared<VideoGraphData>();
    transformData = std::make_shared<ImageData>();

    //input regulation unsigned int
    QRegExpValidator* intPos = new QRegExpValidator(QRegExp("\\d*"), this);

    corner1X->setValidator(intPos);
    corner1Y->setValidator(intPos);
    corner2X->setValidator(intPos);
    corner2Y->setValidator(intPos);
    corner3X->setValidator(intPos);
    corner3Y->setValidator(intPos);
    corner4X->setValidator(intPos);
    corner4Y->setValidator(intPos);

    //connections
    connect(corner1X , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner1Y , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner2X , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner2Y , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner3X , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner3Y , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner4X , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(corner4Y , SIGNAL(editingFinished()), this, SLOT(preCheck()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(&functWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(multiThreadedUpdate()));

    //build layout
    layout->addWidget(coords1 ,1,1);
    layout->addWidget(corner1X ,1,2);
    layout->addWidget(comma1 ,1,3);
    layout->addWidget(corner1Y ,1,4);
    layout->addWidget(coords2 ,2,1);
    layout->addWidget(corner2X ,2,2);
    layout->addWidget(comma2 ,2,3);
    layout->addWidget(corner2Y ,2,4);
    layout->addWidget(coords3 ,3,1);
    layout->addWidget(corner3X ,3,2);
    layout->addWidget(comma3 ,3,3);
    layout->addWidget(corner3Y ,3,4);
    layout->addWidget(coords4 ,4,1);
    layout->addWidget(corner4X ,4,2);
    layout->addWidget(comma4 ,4,3);
    layout->addWidget(corner4Y ,4,4);

    layout->addWidget(progressBar,5,1,1,4);
    layout->addWidget(progressText,6,1,1,4);
    window->setLayout(layout);

    buildContextWindow();
}


unsigned int PerspectiveRectify::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType PerspectiveRectify::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> PerspectiveRectify::outData(PortIndex port)
{
    return videoOut;
}

void PerspectiveRectify::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState PerspectiveRectify::validationState() const
{
    return modelValidationState;
}

QString PerspectiveRectify::validationMessage() const
{
    return modelValidationError;
}

QJsonObject PerspectiveRectify::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["isReady"] = isReady;
    dataJson["Corner1X"] = Corner1x;
    dataJson["Corner1Y"] = Corner1y;
    dataJson["Corner2X"] = Corner2x;
    dataJson["Corner2Y"] = Corner2y;
    dataJson["Corner3X"] = Corner3x;
    dataJson["Corner3Y"] = Corner3y;
    dataJson["Corner4X"] = Corner4x;
    dataJson["Corner4Y"] = Corner4y;

    return dataJson;
}

void PerspectiveRectify::restore(const QJsonObject & json)
{
    if(json.contains("isReady")){
        isReady = json["isReady"].toBool();
    }
    if(json.contains("Corner1X")){
        corner1X->setText(QString::number(json["Corner1X"].toInt()));
    }
    if(json.contains("Corner1Y")){
        corner1Y->setText(QString::number(json["Corner1Y"].toInt()));
    }
    if(json.contains("Corner2X")){
        corner2X->setText(QString::number(json["Corner2X"].toInt()));
    }
    if(json.contains("Corner2Y")){
        corner2Y->setText(QString::number(json["Corner2Y"].toInt()));
    }
    if(json.contains("Corner3X")){
        corner3X->setText(QString::number(json["Corner3X"].toInt()));
    }
    if(json.contains("Corner3Y")){
        corner3Y->setText(QString::number(json["Corner3Y"].toInt()));
    }
    if(json.contains("Corner4X")){
        corner4X->setText(QString::number(json["Corner4X"].toInt()));
    }
    if(json.contains("Corner4Y")){
        corner4Y->setText(QString::number(json["Corner4Y"].toInt()));
    }

    preCheck();
}

cv::Mat multiThreadedPerspective(const cv::Mat &in){
    cv::Mat output;

    cv::Mat transform = LinkManager::instance()->getImageData("PRIVATEtransform")->_image;
    cv::warpPerspective(in,output,transform,in.size());

    return output;
}


void PerspectiveRectify::processData()
{
    progressText->setText("Processing...");
    videoOut->data().clear();

    float srcQ[4][2], dstQ[4][2];

    srcQ[0][0] = Corner1x;
    srcQ[0][1] = Corner1y;
    srcQ[1][0] = Corner2x;
    srcQ[1][1] = Corner2y;
    srcQ[2][0] = Corner3x;
    srcQ[2][1] = Corner3y;
    srcQ[3][0] = Corner4x;
    srcQ[3][1] = Corner4y;

    dstQ[0][0] = 0;
    dstQ[0][1] = 0;
    dstQ[1][0] = videoIn->_video[1].cols - 1;
    dstQ[1][1] = 0;
    dstQ[2][0] = videoIn->_video[1].cols - 1;
    dstQ[2][1] = videoIn->_video[1].rows - 1;
    dstQ[3][0] = 0;
    dstQ[3][1] = videoIn->_video[1].rows - 1;

    cv::Mat src = cv::Mat(4,2,CV_32FC1, srcQ);
    cv::Mat dst = cv::Mat(4,2,CV_32FC1, dstQ);

    transformData->_image = cv::getPerspectiveTransform(src,dst);

    LinkManager::instance()->sendData(transformData, "PRIVATEtransform");

    //setup progress bar parameters

    QList<cv::Mat> images = QList<cv::Mat>::fromVector(QVector<cv::Mat>::fromStdVector(videoIn->_video));

    funct = QtConcurrent::mapped(images, multiThreadedPerspective);

    functWatcher.setFuture(funct);

    progressBar->setMaximum(functWatcher.progressMaximum());
    progressBar->setValue(0);

}


void PerspectiveRectify::preCheck(){

    if(!corner1X->text().isEmpty()){
        Corner1x = corner1X->text().toInt();
    }
    if(!corner1Y->text().isEmpty()){
        Corner1y = corner1Y->text().toInt();
    }
    if(!corner2X->text().isEmpty()){
        Corner2x = corner2X->text().toInt();
    }
    if(!corner2Y->text().isEmpty()){
        Corner2y = corner2Y->text().toInt();
    }
    if(!corner3X->text().isEmpty()){
        Corner3x = corner3X->text().toInt();
    }
    if(!corner3Y->text().isEmpty()){
        Corner3y = corner3Y->text().toInt();
    }
    if(!corner4X->text().isEmpty()){
        Corner4x = corner4X->text().toInt();
    }
    if(!corner4Y->text().isEmpty()){
        Corner4y = corner4Y->text().toInt();
    }

    if(videoIn && videoIn->isReady){
        if(Corner1x < 0 || Corner1x > videoIn->_video[1].cols-1){
            Corner1x = 0;
            corner1X->setText(QString::number(Corner1x));
        }
        if( Corner4x < 0 || Corner4x > videoIn->_video[1].cols-1){
            Corner4x = 0;
            corner4X->setText(QString::number(Corner4x));
        }
        if( Corner3x < 0 || Corner3x > videoIn->_video[1].cols-1){
            Corner3x = videoIn->_video[1].cols-1;
            corner3X->setText(QString::number(Corner3x));
        }
        if(Corner2x < 0 || Corner2x > videoIn->_video[1].cols-1){
            Corner2x = videoIn->_video[1].cols-1;
            corner2X->setText(QString::number(Corner2x));
        }

        if(Corner1y < 0 || Corner1y > videoIn->_video[1].rows-1){
            Corner1y = 0;
            corner1Y->setText(QString::number(Corner1y));
        }
        if(Corner2y < 0 || Corner2y > videoIn->_video[1].rows-1){
            Corner2y = 0;
            corner2Y->setText(QString::number(Corner2y));
        }
        if( Corner4y < 0 || Corner4y > videoIn->_video[1].rows-1){
            Corner4y = videoIn->_video[1].rows-1;
            corner4Y->setText(QString::number(Corner4y));
        }
        if(Corner3y < 0 || Corner3y > videoIn->_video[1].rows-1){
            Corner3y = videoIn->_video[1].rows-1;
            corner3Y->setText(QString::number(Corner3y));
        }
    }

    if(Corner1x != -1 && Corner1y != -1 &&Corner2x != -1 &&Corner2y != -1 &&Corner3x != -1 &&Corner3y != -1 &&Corner4x != -1 &&Corner4y != -1){
        isReady = true;
    }else{
        isReady = false;
    }

    if(videoIn && videoIn->isReady && isReady && active &&isReady){
        processData();
    }else{
        if(videoOut){videoOut->unready();}

    }
}

void PerspectiveRectify::ShowContextMenu(const QPoint &pos)
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

void PerspectiveRectify::multiThreadedUpdate()
{
    progressText->setText(QString::number(functWatcher.progressValue()) + " Of " + QString::number(functWatcher.progressMaximum()) + " Processing...");
    progressBar->setValue(functWatcher.progressValue());

}

void PerspectiveRectify::multiThreadedFinished()
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

