#include "trainemotionmodels.h"
#include <QFileDialog>
#include <opencv2/video.hpp>


TrainEmotionModel::TrainEmotionModel()
{
    window =  new QWidget;
    layout = new QVBoxLayout;

    button = new QPushButton("Train");
    modelSelection = new QPushButton("Test Model");
    progressBar = new QLabel("Inactive");

    connect(button, SIGNAL(clicked(bool)), this, SLOT(preCheck()));
    //connect(modelSelection, SIGNAL(clicked(bool)), this, SLOT(testModel()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(button);
    layout->addWidget(modelSelection);
    layout->addWidget(progressBar);

    window->setLayout(layout);
    buildContextWindow();

}

TrainEmotionModel::~TrainEmotionModel()
{

}

unsigned int TrainEmotionModel::nPorts(PortType portType) const
{
    unsigned int result;

    switch (portType)
    {
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 0;
        break;
    default:
        break;
    }
    return result;
}

NodeDataType TrainEmotionModel::dataType(PortType, PortIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> TrainEmotionModel::outData(PortIndex port)
{
    return 0;
}

void TrainEmotionModel::processData()
{
    progressBar->setText("Processing...");

    //multiThreadedProcess();

    funct = QtConcurrent::run(this, &TrainEmotionModel::multiThreadedProcess);
    functWatcher.setFuture(funct);

}


void TrainEmotionModel::preCheck()
{
    if(active){
        processData();
    }
}

void TrainEmotionModel::ShowContextMenu(const QPoint &pos)
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


void TrainEmotionModel::multiThreadedProcess()
{
    //vector<Mat> trainingData, testingData;
    //vector<int> trainingLabels, testingLabels;
    vector<string> files;

    //get all files into a vector of mat
    LOG_JOHN() << "Begin Globbing Files";
    glob("C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/yolo/aligned/*.jpg", files);
    //parse all labels into a vector of int

    LOG_JOHN() << "Load File Labels";
    ifstream labelFile;
    string line;
    int fileNum = 0;
    labelFile.open("C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/yolo/list_patition_label.txt");
    if(labelFile.is_open()){
        LOG_JOHN() << "Begin Data Parsing";

        while(getline(labelFile, line)){
            //this will tell us if the image is for testing
            if(line[1] == 'e'){
                //these are testing labels
                testingLabels.push_back(line[22]-48);
                //get file from glob
                Mat image = imread(files[fileNum]);
                Mat grey;
                cvtColor(image, grey, COLOR_BGR2GRAY);          //this is so expensive, i need to do this in preprocessing
                testingData.push_back(grey);

            }else{
                //these are training labels
                trainingLabels.push_back(line[24]-48);
                //get file from glob
                Mat image = imread(files[fileNum]);
                Mat grey;
                cvtColor(image, grey, COLOR_BGR2GRAY);          //this is so expensive, i need to do this in preprocessing
                trainingData.push_back(grey);
            }
            fileNum++;

            LOG_JOHN() << "Parsed File " << to_string(fileNum).c_str() << " of " << to_string(files.size()).c_str() ;
        }

        LOG_JOHN() << "Done Data Parsing";
    }
    labelFile.close();

    fishface = face::FisherFaceRecognizer::load<cv::face::FisherFaceRecognizer>("C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeFisher.yml");
    LOG_JOHN() << "Loaded 1";
    eigenface = face::EigenFaceRecognizer::load<cv::face::EigenFaceRecognizer>("C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeEigen.yml");
    LOG_JOHN() << "Loaded 2";
    lbphface = face::LBPHFaceRecognizer::load<cv::face::LBPHFaceRecognizer>("C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeLBPH.yml");
    LOG_JOHN() << "Loaded 3";

    vector<int> correct = {0,0,0,0,0,0,0};
    vector<int> incorrect = {0,0,0,0,0,0,0};


/*
    LOG_JOHN() << "Begin Training1";
    fishface = face::FisherFaceRecognizer::create();
    fishface->train(trainingData, trainingLabels);
    LOG_JOHN() << "Done Training1";
    QString path ="C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeFisher.yml";
    fishface->save(path.toStdString());
    LOG_JOHN() << "Saved";


    LOG_JOHN() << "Begin Training2";
    eigenface = face::EigenFaceRecognizer::create();
    eigenface->train(trainingData, trainingLabels);
    LOG_JOHN() << "Done Training2";
    path ="C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeEigen.yml";
    eigenface->save(path.toStdString());
    LOG_JOHN() << "Saved";


    LOG_JOHN() << "Begin Training3";
    lbphface = face::LBPHFaceRecognizer::create();
    lbphface->train(trainingData, trainingLabels);
    LOG_JOHN() << "Done Training3";
    path ="C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/dataTypeLBPH.yml";
    lbphface->save(path.toStdString());
    LOG_JOHN() << "Saved";
*/

    for (unsigned int counter = 0; counter < testingData.size(); counter++) {
        int predictedLabel = -1;
        double confidence = 0.0;
        valuePercentages = fishface->prediction(testingData[counter], predictedLabel, confidence);

        //LOG_JOHN() << "Values";
        //LOG_JOHN() << "Surprise: " + QString::number(valuePercentages.at(1));
        //LOG_JOHN() << "Fear: "+ QString::number(valuePercentages.at(2));
        //LOG_JOHN() << "Disgust: "+ QString::number(valuePercentages.at(3));
        //LOG_JOHN() << "Happiness: "+ QString::number(valuePercentages.at(4));
        //LOG_JOHN() << "Sadness: "+ QString::number(valuePercentages.at(5));
        //LOG_JOHN() << "Anger: "+ QString::number(valuePercentages.at(6));
        //LOG_JOHN() << "Neutral: "+ QString::number(valuePercentages.at(7));

        if (predictedLabel == testingLabels[counter]) {
            correct[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " was correct.";
        }
        else {
            incorrect[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " vs actual: " + QString::number(testingLabels[counter]);
        }
    }
    LOG_JOHN() << "Surprise: " + QString::number((100*correct[0])/(correct[0] + incorrect[0])) + "% correct";
    LOG_JOHN() << "Fear: "+ QString::number((100*correct[1])/(correct[1] + incorrect[1])) + "% correct";
    LOG_JOHN() << "Disgust: "+ QString::number((100*correct[2])/(correct[2] + incorrect[2])) + "% correct";
    LOG_JOHN() << "Happiness:"  + QString::number((100*correct[3])/(correct[3] + incorrect[3])) + "% correct";
    LOG_JOHN() << "Sadness:" + QString::number((100*correct[4])/(correct[4] + incorrect[4])) + "% correct";
    LOG_JOHN() << "Anger: " + QString::number((100*correct[5])/(correct[5] + incorrect[5])) + "% correct";
    LOG_JOHN() << "Neutral: " + QString::number((100*correct[6])/(correct[6] + incorrect[6])) + "% correct";


    correct = {0,0,0,0,0,0,0};
    incorrect = {0,0,0,0,0,0,0};

    for (unsigned int counter = 0; counter < testingData.size(); counter++) {
        int predictedLabel = -1;
        double confidence = 0.0;
        valuePercentages = eigenface->prediction(testingData[counter], predictedLabel, confidence);

        //LOG_JOHN() << "Values";
        //LOG_JOHN() << "Surprise: " + QString::number(valuePercentages.at(1));
        //LOG_JOHN() << "Fear: "+ QString::number(valuePercentages.at(2));
        //LOG_JOHN() << "Disgust: "+ QString::number(valuePercentages.at(3));
        //LOG_JOHN() << "Happiness: "+ QString::number(valuePercentages.at(4));
        //LOG_JOHN() << "Sadness: "+ QString::number(valuePercentages.at(5));
        //LOG_JOHN() << "Anger: "+ QString::number(valuePercentages.at(6));
        //LOG_JOHN() << "Neutral: "+ QString::number(valuePercentages.at(7));

        if (predictedLabel == testingLabels[counter]) {
            correct[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " was correct.";
        }
        else {
            incorrect[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " vs actual: " + QString::number(testingLabels[counter]);
        }
    }
    LOG_JOHN() << "Surprise: " + QString::number((100*correct[0])/(correct[0] + incorrect[0])) + "% correct";
    LOG_JOHN() << "Fear: "+ QString::number((100*correct[1])/(correct[1] + incorrect[1])) + "% correct";
    LOG_JOHN() << "Disgust: "+ QString::number((100*correct[2])/(correct[2] + incorrect[2])) + "% correct";
    LOG_JOHN() << "Happiness:"  + QString::number((100*correct[3])/(correct[3] + incorrect[3])) + "% correct";
    LOG_JOHN() << "Sadness:" + QString::number((100*correct[4])/(correct[4] + incorrect[4])) + "% correct";
    LOG_JOHN() << "Anger: " + QString::number((100*correct[5])/(correct[5] + incorrect[5])) + "% correct";
    LOG_JOHN() << "Neutral: " + QString::number((100*correct[6])/(correct[6] + incorrect[6])) + "% correct";

    correct = {0,0,0,0,0,0,0};
    incorrect = {0,0,0,0,0,0,0};

    for (unsigned int counter = 0; counter < testingData.size(); counter++) {
        int predictedLabel = -1;
        double confidence = 0.0;
        valuePercentages = lbphface->prediction(testingData[counter], predictedLabel, confidence);

        //LOG_JOHN() << "Values";
        //LOG_JOHN() << "Surprise: " + QString::number(valuePercentages.at(1));
        //LOG_JOHN() << "Fear: "+ QString::number(valuePercentages.at(2));
        //LOG_JOHN() << "Disgust: "+ QString::number(valuePercentages.at(3));
        //LOG_JOHN() << "Happiness: "+ QString::number(valuePercentages.at(4));
        //LOG_JOHN() << "Sadness: "+ QString::number(valuePercentages.at(5));
        //LOG_JOHN() << "Anger: "+ QString::number(valuePercentages.at(6));
        //LOG_JOHN() << "Neutral: "+ QString::number(valuePercentages.at(7));

        if (predictedLabel == testingLabels[counter]) {
            correct[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " was correct.";
        }
        else {
            incorrect[testingLabels[counter]-1]++;
            LOG_JOHN() << QString::number(predictedLabel) + " vs actual: " + QString::number(testingLabels[counter]);
        }
    }
    LOG_JOHN() << "Surprise: " + QString::number((100*correct[0])/(correct[0] + incorrect[0])) + "% correct";
    LOG_JOHN() << "Fear: "+ QString::number((100*correct[1])/(correct[1] + incorrect[1])) + "% correct";
    LOG_JOHN() << "Disgust: "+ QString::number((100*correct[2])/(correct[2] + incorrect[2])) + "% correct";
    LOG_JOHN() << "Happiness:"  + QString::number((100*correct[3])/(correct[3] + incorrect[3])) + "% correct";
    LOG_JOHN() << "Sadness:" + QString::number((100*correct[4])/(correct[4] + incorrect[4])) + "% correct";
    LOG_JOHN() << "Anger: " + QString::number((100*correct[5])/(correct[5] + incorrect[5])) + "% correct";
    LOG_JOHN() << "Neutral: " + QString::number((100*correct[6])/(correct[6] + incorrect[6])) + "% correct";



/*
    if((100*correct)/(correct + incorrect) > rateThreshold){
        QString path ="C:/projects/Shotcut/src/shotcut/opencv/deeplearningdata/emotion/ModelsImproved/" + QString::number((100*correct)/(correct + incorrect))  + "-" + QString::number(coun) + ".yml";
        fishface->save(path.toStdString());
        LOG_JOHN() << "Saved";
        coun++;
    }
*/

}

void TrainEmotionModel::multiThreadedFinished()
{
    LOG_JOHN() << "Emitted Update";
    progressBar->setText("Finished");
}

