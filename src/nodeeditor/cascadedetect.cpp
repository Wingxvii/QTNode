#include "cascadedetect.h"

CascadeDetect::CascadeDetect()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");
    scaleInput = new QLineEdit();
    scaleDisplay = new QLabel("Scale");
    cascadeSelection = new QListWidget();

    //init out port
    videoOut = std::make_shared<VideoGraphData>();

    //input regulation
    //signed int
    doublepos = new QRegExpValidator(QRegExp("/^[0-9]+(\\.[0-9]+)?$"), this);
    scaleInput->setValidator(doublepos);


    cascadeSelection->addItem("Eye");
    cascadeSelection->addItem("Frontal Cat Face");
    cascadeSelection->addItem("Frontal Cat Face Extended");
    cascadeSelection->addItem("Frontal Face Alternate");
    cascadeSelection->addItem("Frontal Face Alternate 2");
    cascadeSelection->addItem("Frontal Face");
    cascadeSelection->addItem("Full Body");
    cascadeSelection->addItem("Left Eye 2 Splits");
    cascadeSelection->addItem("Right Eye 2 Splits");
    cascadeSelection->addItem("Russina Licence Plate");
    cascadeSelection->addItem("Lower Body");
    cascadeSelection->addItem("Upper Body");
    cascadeSelection->addItem("Profile Face");
    cascadeSelection->addItem("Right Eye 2 Splits");
    cascadeSelection->addItem("Smile");


    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //build layout
    layout->addWidget(progressBar,1,1);

    window->setLayout(layout);
    buildContextWindow();
}

unsigned int CascadeDetect::nPorts(QtNodes::PortType PortType) const
{
    unsigned int result = 1;

    switch(PortType){
    case QtNodes::PortType::In:
        result = 1;
        break;
    case QtNodes::PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

QtNodes::NodeDataType CascadeDetect::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> CascadeDetect::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return videoOut;
        break;

    }
}

void CascadeDetect::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            videoOut = std::make_shared<VideoGraphData>();
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
}

QtNodes::NodeValidationState CascadeDetect::validationState() const
{
    return modelValidationState;

}

QString CascadeDetect::validationMessage() const
{
    return modelValidationError;

}

QJsonObject CascadeDetect::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;

}

void CascadeDetect::restore(const QJsonObject &)
{
    preCheck();
}

void CascadeDetect::processData()
{
    progressBar->setText("Processing...");
    scale = scaleInput->selectedText().toDouble();

    funct = QtConcurrent::run(this, &CascadeDetect::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void CascadeDetect::preCheck()
{
    if(videoIn && videoIn->isReady && active && !cascade1.empty()){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void CascadeDetect::ShowContextMenu(const QPoint &pos)
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

void CascadeDetect::multiThreadedProcess()
{
    std::vector<cv::Rect> faces, faces2;
    cv::Mat smallImg;

    for(cv::Mat frame : videoIn->_video){
        resize(frame, smallImg, cv::Size(), 1/scale, 1/scale,cv::INTER_LINEAR);

        cascade1.detectMultiScale(smallImg, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30,30));

        for (size_t i = 0; i < faces.size(); i++)
            {
                cv::Rect r = faces[i];
                cv::Mat smallImgROI;
                std::vector<cv::Rect> nestedObjects;
                cv::Point center;
                cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool
                int radius;

                double aspect_ratio = (double)r.width / r.height;
                if (0.75 < aspect_ratio && aspect_ratio < 1.3)
                {
                    center.x = cvRound((r.x + r.width*0.5)*scale);
                    center.y = cvRound((r.y + r.height*0.5)*scale);
                    radius = cvRound((r.width + r.height)*0.25*scale);
                    circle(frame, center, radius, color, 3, 8, 0);
                }
                else
                    rectangle(frame, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                        cv::Point(cvRound((r.x + r.width - 1)*scale),
                            cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);

                /*if (nestedCascade.empty())
                    continue;
                smallImgROI = smallImg(r);

                // Detection of eyes int the input image
                nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
                    0 | CASCADE_SCALE_IMAGE, Size(30, 30));

                // Draw circles around eyes
                for (size_t j = 0; j < nestedObjects.size(); j++)
                {
                    Rect nr = nestedObjects[j];
                    center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
                    center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
                    radius = cvRound((nr.width + nr.height)*0.25*scale);
                    circle(img, center, radius, color, 3, 8, 0);
                }
                */
            }
    }

}

void CascadeDetect::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);
}

void CascadeDetect::selectCascade()
{

}

