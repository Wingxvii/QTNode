#include "calculateopticalflow.h"

CalculateOpticalFlow::CalculateOpticalFlow()
{
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    videoOut = std::make_shared<VideoGraphData>();
    pointsOut = std::make_shared<PointsData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);


    buildContextWindow();

}

unsigned int CalculateOpticalFlow::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType CalculateOpticalFlow::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        if(portIndex == 0){
            return VideoGraphData().type();
        }else{
            return PointData().type();
        }
    }else{
        if(portIndex == 0){
            return VideoGraphData().type();
        }else{
            return PointsData().type();
        }
    }
}

std::shared_ptr<QtNodes::NodeData> CalculateOpticalFlow::outData(QtNodes::PortIndex port)
{
    if(port == 0){
        return videoOut;
    }else{
        return pointsOut;
    }
}

void CalculateOpticalFlow::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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
    pointsIn = std::dynamic_pointer_cast<PointData>(data);
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

QtNodes::NodeValidationState CalculateOpticalFlow::validationState() const
{
    return modelValidationState;

}

QString CalculateOpticalFlow::validationMessage() const
{
    return modelValidationError;
}

void CalculateOpticalFlow::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &CalculateOpticalFlow::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void CalculateOpticalFlow::preCheck()
{
    if(videoIn && videoIn->isReady && pointsIn && pointsIn->isReady && active){
        processData();
    }else{
        videoOut->unready();
        pointsOut->unready();
    }


}

void CalculateOpticalFlow::ShowContextMenu(const QPoint &pos)
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

void CalculateOpticalFlow::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;
    std::vector<std::vector<cv::Point2f>> tempPoints;
    cv::Mat greyFrame , oldFrame, img;
    oldFrame = videoIn->_video[0];
    std::vector<cv::Point2f> p0, p1;
    p0 = pointsIn->_pointList;
    std::vector<uchar> status;
    std::vector<float> err;
    cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);
    cv::Mat mask = cv::Mat::zeros(oldFrame.size(), oldFrame.type());

    cvtColor(oldFrame, oldFrame, 6);

    for(cv::Mat frame : videoIn->_video){

        std::vector<cv::Point2f> goodPoints;

        cvtColor(frame, greyFrame, 6);
        calcOpticalFlowPyrLK(oldFrame, greyFrame, p0, p1, status, err, cv::Size(15, 15), 2, criteria);

        for (uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if (status[i] == 1) {
                goodPoints.push_back(p1[i]);
                // draw the tracks
                line(mask, p1[i], p0[i], cv::Scalar(0,255,0), 2);
                circle(frame, p1[i], 5, cv::Scalar(0,255,0), -1);
            }
        }
        cv::add(frame, mask, img);
        temp.push_back(img.clone());
        tempPoints.push_back(goodPoints);
        //update points
        oldFrame = greyFrame.clone();
        p0 = goodPoints;
        LOG_JOHN() << "Frame Processed:" << temp.size();

    }
    pointsOut->_pointList = tempPoints;
    videoOut->_video = temp;
}

void CalculateOpticalFlow::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    pointsOut->ready();
    emit dataUpdated(0);
    emit dataUpdated(1);
}
