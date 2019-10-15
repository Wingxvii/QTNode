#include "deepimageclassification.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>


DeepImageClassification::DeepImageClassification()
{
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");
    modelSelection = new QComboBox();
    confidenceInput = new QSlider(Qt::Horizontal);
    supressionInput = new QSlider(Qt::Horizontal);
    confidenceLabel = new QLabel("Confidence: ");
    supressionLabel = new QLabel("Supression: ");

    fileClassesLabel = new QLabel("Classes: Empty");
    fileConfigLabel = new QLabel("Config : Empty");
    fileWeightsLabel = new QLabel("Weights : Empty");

    fileClasses = new QPushButton("Choose Classes File");
    fileConfig = new QPushButton("Choose Config File");
    fileWeights = new QPushButton("Choose Weights File");


    confidenceInput->setMinimum(0);
    confidenceInput->setMaximum(10);
    confidenceInput->setValue(5);


    supressionInput->setMinimum(0);
    supressionInput->setMaximum(10);
    supressionInput->setValue(4);

    setupModels();

    dataOut = std::make_shared<DetectionBoxesData>();

    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(modelSelection, SIGNAL(activated(int)), this, SLOT(selectModels()));
    connect(fileClasses, SIGNAL(clicked(bool)), this, SLOT(selectClasses()));
    connect(fileConfig, SIGNAL(clicked(bool)), this, SLOT(selectConfig()));
    connect(fileWeights, SIGNAL(clicked(bool)), this, SLOT(selectWeights()));

    layout->addWidget(progressBar,6,1);

    //layout->addWidget(modelSelection,1,1,1,2);
    layout->addWidget(fileClassesLabel,1,1);
    layout->addWidget(fileClasses,1,2);
    layout->addWidget(fileConfigLabel,2,1);
    layout->addWidget(fileConfig,2,2);
    layout->addWidget(fileWeightsLabel,3,1);
    layout->addWidget(fileWeights,3,2);

    layout->addWidget(confidenceLabel,4,1);
    layout->addWidget(confidenceInput,4,2);

    layout->addWidget(supressionLabel,5,1);
    layout->addWidget(supressionInput,5,2);

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int DeepImageClassification::nPorts(QtNodes::PortType PortType) const
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

QtNodes::NodeDataType DeepImageClassification::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> DeepImageClassification::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
        return dataOut;
        break;

    }
}

void DeepImageClassification::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
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

QtNodes::NodeValidationState DeepImageClassification::validationState() const
{
    return modelValidationState;

}

QString DeepImageClassification::validationMessage() const
{
    return modelValidationError;

}

void DeepImageClassification::processData()
{
    LOG_JOHN() << "Started Process";
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &DeepImageClassification::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void DeepImageClassification::preCheck()
{

    confThreshold = confidenceInput->value() / 10.0f;
    nmsThreshold = supressionInput->value() / 10.0f;

    LOG_JOHN() << confThreshold;
    LOG_JOHN() << nmsThreshold;

    if(videoIn && videoIn->isReady && active && !classesFile.empty() && !modelConfig.empty() && !modelWeights.empty()){
        processData();
    }
}

void DeepImageClassification::ShowContextMenu(const QPoint &pos)
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



void DeepImageClassification::multiThreadedProcess()
{
    cv::Mat blob;

    //load network
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_OPENCL);


    //iterate through frames
    for(cv::Mat frame : videoIn->_video){
        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        if(frame.empty()){
            //DONE
            break;
        }
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0,0,0), true, false);

        net.setInput(blob);

        if (names.empty())
        {
            //Get the indices of the output layers, i.e. the layers with unconnected outputs
            std::vector<int> outLayers = net.getUnconnectedOutLayers();

            //get the names of all the layers in the network
            std::vector<cv::String> layersNames = net.getLayerNames();

            // Get the names of the output layers in names
            names.resize(outLayers.size());
            for (size_t i = 0; i < outLayers.size(); ++i){
                names[i] = layersNames[outLayers[i] - 1];
                LOG_JOHN() << "Names Added";
            }
        }
        std::vector<cv::Mat> outs;
        net.forward(outs, names);

        //post process

        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        for (size_t i = 0; i < outs.size(); ++i){
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols){
                cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                cv::Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

                if (confidence > confThreshold)
                {
                    LOG_JOHN() << "found!";

                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));
                }
            }
        }

        // Perform non maximum suppression to eliminate redundant overlapping boxes with
        // lower confidences
        std::vector<cv::Rect> temp = std::vector<cv::Rect>();
        std::vector<float> tempConf = std::vector<float>();
        std::vector<std::string> tempNames = std::vector<std::string>();
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
        for (size_t i = 0; i < indices.size(); ++i)
        {
            int idx = indices[i];

            //load indexed data
            if (!classes.empty())
            {
                CV_Assert(classIds[idx] < (int)classes.size());
                tempNames.push_back(classes[classIds[idx]]);
            }

            temp.push_back(boxes[idx]);
            tempConf.push_back(confidences[idx]);
        }

        //send data to out port
        dataOut->_boxes.push_back(temp);
        dataOut->_conf.push_back(tempConf);
        dataOut->_names.push_back(tempNames);
        LOG_JOHN() << "Frame Done: ";// + dataOut->_boxes.size();

    }
}

void DeepImageClassification::multiThreadedFinished()
{
    progressBar->setText("Finished");
    dataOut->ready();
    emit dataUpdated(0);

}

void DeepImageClassification::setupModels()
{
    modelSelection->addItem("YOLOv3 COCO Models");

}

void DeepImageClassification::selectModels()
{
    if(modelSelection->currentIndex() == 0){ //YOLOv3 COCO Models
        QDir direct = QDir();

        classesFile = direct.filePath("deepnetworkdata/yolo/coco.names").toStdString();
        modelConfig =  direct.filePath("deepnetworkdata/yolo/yolov3.cfg").toStdString();
        modelWeights =  direct.filePath("deepnetworkdata/yolo/yolov3.weights").toStdString();

        //load classes
        classes.clear();
        std::ifstream ifs(classesFile.c_str());
        std::string line;
        while (std::getline(ifs, line)) classes.push_back(line);


        LOG_JOHN() << "Loaded COCO Model";
    }
}

void DeepImageClassification::selectClasses()
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

        preCheck();
    }
}

void DeepImageClassification::selectConfig()
{
    modelConfig = QFileDialog::getOpenFileName(0, tr("Open Config File"), "C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata", tr("Class File (*.cfg)")).toStdString();
    if(modelConfig.empty()){
        fileConfigLabel->setText("Config: Empty");
    }else{
        fileConfigLabel->setText("Config: Found");
        preCheck();
    }

}

void DeepImageClassification::selectWeights()
{
    modelWeights = QFileDialog::getOpenFileName(0, tr("Open Classes File"), "C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata", tr("Weight File (*.weights)")).toStdString();
    if(modelWeights.empty()){
        fileWeightsLabel->setText("Weights: Empty");
    }else{
        fileWeightsLabel->setText("Weights: Found");
        preCheck();
    }

}
