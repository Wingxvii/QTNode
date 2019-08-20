#include "displaydetection.h"

DisplayDetection::DisplayDetection()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");
    settings = new QComboBox();

    settings->addItem("Cascade Detection"); //0
    settings->addItem("Deep Object Classification"); //1


    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(settings, SIGNAL(activated(int)), this, SLOT(preCheck()));

    //build layout
    layout->addWidget(progressBar,1,2);
    layout->addWidget(settings,1,1);
    window->setLayout(layout);
    buildContextWindow();
}

unsigned int DisplayDetection::nPorts(QtNodes::PortType PortType) const
{
    unsigned int result = 1;

    switch(PortType){
    case QtNodes::PortType::In:
        result = 2;
        break;
    case QtNodes::PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

QtNodes::NodeDataType DisplayDetection::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex == 1){
        return DetectionBoxesData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> DisplayDetection::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return videoOut;
        break;

    }
}

void DisplayDetection::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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
        dataIn = std::dynamic_pointer_cast<DetectionBoxesData>(data);
            if(dataIn){
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

QtNodes::NodeValidationState DisplayDetection::validationState() const
{
    return modelValidationState;

}

QString DisplayDetection::validationMessage() const
{
    return modelValidationError;

}

QJsonObject DisplayDetection::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;

}

void DisplayDetection::restore(const QJsonObject &)
{
    preCheck();
}

void DisplayDetection::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &DisplayDetection::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void DisplayDetection::preCheck()
{



    if(videoIn && videoIn->isReady && dataIn && dataIn->isReady && active ){
        if(settings->currentIndex() ==0 && dataIn->_boxes.empty() ){
            progressBar->setText("Invalid Input Data");

        }else if (settings->currentIndex() ==1 && (dataIn->_boxes.empty() || dataIn->_names.empty() || dataIn->_conf.empty())){
            progressBar->setText("Invalid Input Data");

        }
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void DisplayDetection::ShowContextMenu(const QPoint &pos)
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

void DisplayDetection::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;
    cv::Mat frame;

    if(settings->currentIndex() ==0){
        int scale = dataIn->scale;
        cv::Point center;

        for(int x = 0; x < videoIn->_video.size(); x++){
            frame = videoIn->_video[x];

            for(cv::Rect rect : dataIn->_boxes[x]){
                center.x = cvRound((rect.x + rect.width*0.5)*scale);
                center.y = cvRound((rect.y + rect.height*0.5)*scale);

                cv::circle(frame, center, cvRound((rect.width + rect.height)*0.25*scale), cv::Scalar(255, 0, 0), 3, 8, 0);

            }
        temp.push_back(frame.clone());

        }
        videoOut->_video = temp;
    }else if(settings->currentIndex() == 1){
        for(int x = 0; x < videoIn->_video.size(); x++){
            LOG_JOHN() << "Drew frame: " << QString::number(x);
            frame = videoIn->_video[x];
            for(int y = 0; y < dataIn->_boxes[x].size(); y++){
                LOG_JOHN() << "Drew Box: " << QString::number(y) << " in frame: " << QString::number(x);
                cv::Rect rect = dataIn->_boxes[x][y];
                float conf = dataIn->_conf[x][y];
                std::string name = dataIn->_names[x][y];
                cv::rectangle(frame, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(255, 178, 50), 3);
                std::string label = cv::format("%.2f", conf);
                label = name + ":" + label;
                int baseLine;
                cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
                int top = cv::max(rect.y, labelSize.height);
                rectangle(frame, cv::Point(rect.x, top - round(1.5*labelSize.height)), cv::Point(rect.x + round(1.5*labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
                cv::putText(frame, label, cv::Point(rect.x, rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0),1);

            }

            temp.push_back(frame.clone());
        }
        videoOut->_video = temp;

    }

}

void DisplayDetection::multiThreadedFinished()
{
    progressBar->setText("Finished");
    if(!videoOut->_video.empty()){
    videoOut->ready();
    }
    emit dataUpdated(0);
}
