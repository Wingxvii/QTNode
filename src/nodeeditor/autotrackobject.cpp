#include "autotrackobject.h"

AutoTrackObject::AutoTrackObject()
{
    layout = new QGridLayout;
    window =  new QWidget;

    progressBar = new QLabel("Inactive");
    rangeMinLabel = new QLabel("Min Range Denominator: ");
    rangeMaxLabel = new QLabel("Max Range Denominator: ");
    rangeMinInput = new QLineEdit();
    rangeMaxInput = new QLineEdit();

    objectsOut = std::make_shared<DetectionBoxesData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //input regulation
    //double
    doublepos = new QDoubleValidator();

    rangeMaxInput->setValidator(doublepos);
    rangeMinInput->setValidator(doublepos);

    layout->addWidget(progressBar,3,1);
    layout->addWidget(rangeMinLabel,1,1);
    layout->addWidget(rangeMinInput,1,2);
    layout->addWidget(rangeMaxLabel,2,1);
    layout->addWidget(rangeMaxInput,2,2);

    window->setLayout(layout);

    buildContextWindow();

}

unsigned int AutoTrackObject::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> AutoTrackObject::outData(PortIndex port){
    return objectsOut;
}

void AutoTrackObject::setInData(std::shared_ptr<NodeData> data, int location){
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Video data was not found";
        }

}
NodeDataType AutoTrackObject::dataType(PortType portType, PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return DetectionBoxesData().type();
}

NodeValidationState AutoTrackObject::validationState() const
{
    return modelValidationState;
}

QString AutoTrackObject::validationMessage() const
{
    return modelValidationError;
}

QJsonObject AutoTrackObject::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();
    dataJson["rangeMax"] = rangeMax;
    dataJson["rangeMin"] = rangeMin;
    return dataJson;

}

void AutoTrackObject::restore(const QJsonObject & json)
{
    if(json.contains("rangeMax")){
        rangeMaxInput->setText(QString::number(json["rangeMax"].toDouble()));
    }
    if(json.contains("rangeMin")){
        rangeMinInput->setText(QString::number(json["rangeMin"].toDouble()));
    }

    preCheck();
}

void AutoTrackObject::processData()
{
    FrameHeight = videoIn->_video[0].rows;
    FrameWidth = videoIn->_video[0].cols;

    MinObjectArea = (FrameHeight * FrameWidth) / rangeMin;
    MaxObjectArea = (FrameHeight * FrameWidth) / rangeMax;

    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &AutoTrackObject::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void AutoTrackObject::preCheck()
{
    if(rangeMaxInput->text() != 0){
        rangeMax = rangeMaxInput->text().toDouble();
    }
    if(rangeMinInput->text() != 0){
        rangeMin = rangeMinInput->text().toDouble();
    }


    if(videoIn && videoIn->isReady && active){
        processData();
    }
    else{
        if(objectsOut){objectsOut->unready();}
    }

}

void AutoTrackObject::ShowContextMenu(const QPoint &pos)
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

void AutoTrackObject::multiThreadedProcess()
{
    found = false;
    cv::Mat temp;
    std::vector<std::vector<cv::Rect>> allObjs;

    for(cv::Mat threshold : videoIn->_video){
        LOG_JOHN() << "Checking New Frame...";

        std::vector<cv::Rect> frameobjs = std::vector<cv::Rect>();

        threshold.copyTo(temp);
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(temp, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
        //used to find the biggest object

        if(hierarchy.size() > 0){
            int numObjects = hierarchy.size();
            cv::Rect tempPoint;

            if(numObjects < MaxNumObjects){

                LOG_JOHN() << "Objects Discovered:" << numObjects;
                for (int index = 0; index >= 0; index = hierarchy[index][0]) {
                    LOG_JOHN() << "Index" << index;

                    cv::Moments moment = moments((cv::Mat)contours[index]);
                    double area = moment.m00;

                    //if the area is less than 20 px by 20px then it is probably just noise
                    //if the area is the same as the 3/2 of the image size, probably just a bad filter
                    if (area > MinObjectArea && area<MaxObjectArea) {
                        tempPoint.x = moment.m10 / area;
                        tempPoint.y = moment.m01 / area;
                        frameobjs.push_back(tempPoint);
                        found = true;
                        LOG_JOHN() << "Found one";
                    }
                }
            }
        }

        allObjs.push_back(frameobjs);

    }
        objectsOut->_boxes = allObjs;
        objectsOut->ready();
}

void AutoTrackObject::multiThreadedFinished()
{
    if(found){
        progressBar->setText("Finished");
    }else{
        progressBar->setText("No Objects Found");
    }
    LOG_JOHN() << "Converted Successfully";

    emit dataUpdated(0);

}
