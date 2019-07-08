#include "cascadedetect.h"

CascadeDetect::CascadeDetect()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");
    scaleInput = new QLineEdit("1");
    scaleDisplay = new QLabel("Scale");
    cascadeSelection = new QListWidget();
    scaleFactorLabel = new QLabel("Scale Factor Threshold:");
    neigborsLabel = new QLabel("Minimum number of Neighbors: ");
    scaleFactorInput = new QLineEdit("1.2");
    neighborsInput = new QLineEdit("5");

    //init out port
    dataOut = std::make_shared<DetectionBoxesData>();

    setupCascades();

    //input regulation
    //double
    doublepos = new QDoubleValidator();
    //unsigned int
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);

    scaleInput->setValidator(doublepos);
    scaleFactorInput->setValidator(doublepos);
    neighborsInput->setValidator(intPos);

    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(cascadeSelection, SIGNAL(currentRowChanged(int)), this, SLOT(selectCascade(int)));

    //build layout
    layout->addWidget(progressBar,1,1);
    layout->addWidget(scaleDisplay,2,1);
    layout->addWidget(scaleInput,2,2);
    layout->addWidget(scaleFactorLabel,3,1);
    layout->addWidget(scaleFactorInput,3,2);
    layout->addWidget(neigborsLabel,4,1);
    layout->addWidget(neighborsInput,4,2);
    layout->addWidget(cascadeSelection,5,1,1,2);

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
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> CascadeDetect::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return dataOut;
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
            dataOut = std::make_shared<DetectionBoxesData>();
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
    dataJson["scale"] = scale;
    dataJson["cascade"] = selectedRow;
    dataJson["scfact"] = scaleFactor;
    dataJson["neighbors"] = neighbors;
    return dataJson;

}

void CascadeDetect::restore(const QJsonObject & json)
{
    if(json.contains("scale")){
        scaleInput->setText(QString::number(json["scale"].toDouble()));
    }
    if(json.contains("cascade")){
        cascadeSelection->setCurrentRow(json["cascade"].toInt());
        selectCascade(json["cascade"].toInt());
    }
    if(json.contains("scfact")){
        scaleInput->setText(QString::number(json["scfact"].toDouble()));
    }
    if(json.contains("neighbors")){
        scaleInput->setText(QString::number(json["scale"].toInt()));
    }


    preCheck();
}

void CascadeDetect::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &CascadeDetect::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void CascadeDetect::preCheck()
{
    scale = scaleInput->text().toDouble();
    scaleFactor = scaleFactorInput->text().toDouble();
    neighbors = neighborsInput->text().toInt();

    LOG_JOHN() << "Scale: " << scale;

    if(videoIn && videoIn->isReady){

        LOG_JOHN() << "Video is Ready";
    }
    if(!cascade1.empty()){

        LOG_JOHN() << "Cascade is Ready";
    }

    if(videoIn && videoIn->isReady && active && !cascade1.empty()){
        processData();
    }
    else{
        if(dataOut){dataOut->unready();}
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
    std::vector<std::vector<cv::Rect>> temp;

    std::vector<cv::Rect> faces, faces2;
    cv::Mat smallImg, frame;

    for(cv::Mat frame : videoIn->_video){
        //resize(frame, smallImg, cv::Size(0,0), 1/scale, 1/scale,cv::INTER_LINEAR);

        cascade1.detectMultiScale(frame, faces, scaleFactor, neighbors);

        for (size_t i = 0; i < faces.size(); i++)
            {
                cv::Rect r = faces[i];
                //cv::Mat smallImgROI;
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
                    //cv::circle(frame, center, radius, color, 3, 8, 0);
                    LOG_JOHN() << "Cascade Found: (" << center.x << "," << center.y << ")*" << radius;
                }

                /*
                else{
                    rectangle(frame, cv::Point(cvRound(r.x*scale), cvRound(r.y*scale)),
                        cv::Point(cvRound((r.x + r.width - 1)*scale),
                            cvRound((r.y + r.height - 1)*scale)), color, 3, 8, 0);
                }
                */

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
        temp.push_back(faces);
        LOG_JOHN() << "Done " << temp.size() << " Frames";
    }

    dataOut->scale = scale;
    dataOut->_boxes = temp;

}

void CascadeDetect::multiThreadedFinished()
{
    progressBar->setText("Finished");
    dataOut->ready();
    emit dataUpdated(0);
}

void CascadeDetect::selectCascade(int index)
{

    LOG_JOHN() << "Cascade Loaded: " << index;
    selectedRow = index;

    switch(index){
    case 0:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_eye.xml");
        break;
    case 1:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalcatface.xml");
        break;
    case 2:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalcatface_extended.xml");
        break;
    case 3:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalface_default.xml");
        break;
    case 4:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalface_alt.xml");
        break;
    case 5:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalface_alt2.xml");
        break;
    case 6:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_fullbody.xml");
        break;
    case 7:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_lowerbody.xml");
        break;
    case 8:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_upperbody.xml");
        break;
    case 9:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_lefteye_2splits.xml");
        break;
    case 10:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_righteye_2splits.xml");
        break;
    case 11:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_profileface.xml");
        break;
    case 12:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_smile.xml");
        break;
    default:
        cascade1.load("C:/projects/Shotcut/src/shotcut/opencv/cascades/haarcascade_frontalcatface.xml");
        break;
    }


    preCheck();

}

void CascadeDetect::setupCascades()
{
    cascadeSelection->addItem("Eye");
    cascadeSelection->addItem("Frontal Cat Face");
    cascadeSelection->addItem("Frontal Cat Face Extended");
    cascadeSelection->addItem("Frontal Face");
    cascadeSelection->addItem("Frontal Face Alternate");
    cascadeSelection->addItem("Frontal Face Alternate 2");
    cascadeSelection->addItem("Full Body");
    cascadeSelection->addItem("Lower Body");
    cascadeSelection->addItem("Upper Body");
    cascadeSelection->addItem("Left Eye 2 Splits");
    cascadeSelection->addItem("Right Eye 2 Splits");
    cascadeSelection->addItem("Profile Face");
    cascadeSelection->addItem("Smile");
}

