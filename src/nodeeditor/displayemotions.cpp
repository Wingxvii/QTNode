#include "displayemotions.h"

DisplayEmotions::DisplayEmotions()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //build layout
    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);
    buildContextWindow();


}

unsigned int DisplayEmotions::nPorts(QtNodes::PortType PortType) const
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

QtNodes::NodeDataType DisplayEmotions::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In && portIndex == 1){
        return EmotionData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> DisplayEmotions::outData(QtNodes::PortIndex port)
{
    return videoOut;
}

void DisplayEmotions::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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
        dataIn = std::dynamic_pointer_cast<EmotionData>(data);
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


QtNodes::NodeValidationState DisplayEmotions::validationState() const
{
    return modelValidationState;

}

QString DisplayEmotions::validationMessage() const
{
    return modelValidationError;

}

void DisplayEmotions::processData()
{

    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &DisplayEmotions::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void DisplayEmotions::preCheck()
{
    if(videoIn && videoIn->isReady && dataIn && dataIn->isReady && active ){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void DisplayEmotions::multiThreadedProcess()
{
    std::vector<cv::Mat> temp;
    for(int counter = 0; counter < videoIn->_video.size(); counter++){
        cv::Mat tempFrame = videoIn->_video[counter].clone();

        if(dataIn->_valuePercentages[counter].empty()){
            //draw surrounding box
            cv::rectangle(tempFrame, cv::Point(0,0), cv::Point(300, (10 + ((1+dataIn->_labels.size()) * 20))), cv::Scalar(0,0,0), -1);

            for(int counter2 = 0; counter2 < dataIn->_labels.size(); counter2++){
                std::string text = dataIn->_labels[counter2] + std::string(": ") + std::to_string(dataIn->_valuePercentages[counter].at(counter2));
                cv::putText(tempFrame, text, cv::Point(5,(counter2 + 1) * 20), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255), 1);
            }
            cv::putText(tempFrame, "Prominent Emotion: Not Detected", cv::Point(5,120), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255), 1);
            temp.push_back(tempFrame);
            continue;
        }

        int i = 0;
        std::string emotionProm;
        int smallest = dataIn->_valuePercentages[counter].at(0);
        for(int counter2 = 1; counter2 < 5; counter2 ++){
            if(smallest > dataIn->_valuePercentages[counter].at(counter2)){
                smallest = dataIn->_valuePercentages[counter].at(counter2);
                i = counter2;
            }
        }
        emotionProm = dataIn->_labels[i];

        cv::rectangle(tempFrame, cv::Point(0,0), cv::Point(300, (10 + ((1+dataIn->_labels.size()) * 20))), cv::Scalar(0,0,0), -1);

        for(int counter2 = 0; counter2 < dataIn->_labels.size(); counter2++){
            std::string text = dataIn->_labels[counter2] + std::string(": ") + std::to_string(dataIn->_valuePercentages[counter].at(counter2));
            cv::putText(tempFrame, text, cv::Point(5,(counter2 + 1) * 20), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255), 1);
        }
        cv::putText(tempFrame, "Prominent Emotion: " + emotionProm, cv::Point(5,120), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255,255,255), 1);

        temp.push_back(tempFrame);
    }

    videoOut->_video = temp;

}

void DisplayEmotions::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}


void DisplayEmotions::ShowContextMenu(const QPoint &pos)
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

