#include "emotiondetection.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

EmotionDetection::EmotionDetection()
{
    //init everything
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");
    selectModel = new QPushButton("Choose Model File");
    modelSelection = new QLabel("Model : Empty");
    fileClasses = new QPushButton("Choose Classes File");
    fileClassesLabel = new QLabel("Classes: Empty");

    //init out port
    dataOut = std::make_shared<EmotionData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(selectModel, SIGNAL(clicked(bool)), this, SLOT(getModel()));
    connect(fileClasses, SIGNAL(clicked(bool)), this, SLOT(selectClasses()));

    layout->addWidget(progressBar,1,1);
    layout->addWidget(modelSelection,2,1);
    layout->addWidget(selectModel,2,2);
    layout->addWidget(fileClassesLabel,3,1);
    layout->addWidget(fileClasses,3,2);

    window->setLayout(layout);
    buildContextWindow();
}

unsigned int EmotionDetection::nPorts(QtNodes::PortType PortType) const
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

QtNodes::NodeDataType EmotionDetection::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return EmotionData().type();
}

std::shared_ptr<QtNodes::NodeData> EmotionDetection::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return dataOut;
        break;
    }

}

void EmotionDetection::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
}

QtNodes::NodeValidationState EmotionDetection::validationState() const
{
    return modelValidationState;

}

QString EmotionDetection::validationMessage() const
{
    return modelValidationError;

}

void EmotionDetection::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &EmotionDetection::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void EmotionDetection::preCheck()
{
    if(videoIn && videoIn->isReady && !fishface->empty() && !classes.empty() &&active){
        processData();
    }
    else{
        if(dataOut){dataOut->unready();}
    }

}

void EmotionDetection::multiThreadedProcess()
{
    std::string filePath;

    std::map<int, double> valuePercentages;
    int predictedLabel = -1;
    double confidence = 0.0;

    int counter = 0;
    for(cv::Mat frame:  videoIn->_video){
        counter++;
        if(frame.empty()){      //send in -1 for unfound faces
            std::map<int, double> temp;
            dataOut->_valuePercentages.push_back(temp);
            continue;
        }
        cv::resize(frame, frame, cv::Size(100,100));

        valuePercentages = fishface->prediction(frame, predictedLabel, confidence);
        dataOut->_valuePercentages.push_back(valuePercentages);
        dataOut->frameNumbers.push_back(counter);
    }

}

void EmotionDetection::multiThreadedFinished()
{
    progressBar->setText("Finished");
    dataOut->ready();
    emit dataUpdated(0);
}

void EmotionDetection::getModel()
{
    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Choose Model"),QString(), tr("Yaml (*.yml)"));

    fishface = cv::face::FisherFaceRecognizer::load<cv::face::FisherFaceRecognizer>(filename.toStdString());

    modelSelection->setText("Model : Found");
    preCheck();
}

void EmotionDetection::selectClasses()
{
    classesFile = QFileDialog::getOpenFileName(0, tr("Open Classes File"), "C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata", tr("Class File (*.names)")).toStdString();
    if(classesFile.empty()){
        fileClassesLabel->setText("Classes: Empty");
    }else{
        fileClassesLabel->setText("Classes: Found");

        //load classes
        classes.clear();
        std::ifstream ifs(classesFile.c_str());
        std::string line;
        while (std::getline(ifs, line)) classes.push_back(line);

        dataOut->_labels = classes;

        preCheck();
    }

}

void EmotionDetection::ShowContextMenu(const QPoint &pos)
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
