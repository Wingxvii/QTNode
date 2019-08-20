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
    connect(modelSelection, SIGNAL(clicked(bool)), this, SLOT(testModel()));
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



pair<vector<string>, vector<string>> TrainEmotionModel::getFiles(emotions emotion) {
    vector<string> files;
    if (emotion == Neutral) {
        glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/neutral/*.png", files);
    }
    else if (emotion == Anger) {
        glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/anger/*.png", files);
    }
    else if (emotion == Disgust) {
        glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/disgust/*.png", files);
    }
    else if (emotion == Happy) {
        glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/happy/*.png", files);
    }
    else if (emotion == Surprise) {
        glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/surprise/*.png", files);
    }

    time_t current_time;
    current_time = time(NULL);
    auto rng = std::default_random_engine{};
    rng.seed(current_time);

    //shuffles data and seperates into two random lists
    vector<string> training, prediction;
    shuffle(files.begin(), files.end(), rng);
    int eightyPercent = (int)((float)files.size() * 0.8f);
    training = vector<string>(files.begin(), files.begin() + eightyPercent);
    prediction = vector<string>(files.begin() + eightyPercent + 1, files.end());

    /*
    LOG_JOHN() << QString().fromStdString(prediction[0]);
    LOG_JOHN() << QString().fromStdString(prediction[1]);
    LOG_JOHN() << QString().fromStdString(prediction[2]);
    LOG_JOHN() << QString().fromStdString(prediction[3]);
    */

    pair<vector<string>, vector<string>> returnValue;

    returnValue.first = training;
    returnValue.second = prediction;
    return returnValue;
}

void TrainEmotionModel::multiThreadedProcess()
{
    string label;
    emotions emotion;
    vector<Mat> trainingData, predictionData;
    vector<int> trainingLabels, predictionLabels;

    for (int counter = 0; counter < 5; counter++) {

        if (counter == 0) {
            label = "neutral";
            emotion = Neutral;
        }
        else if (counter == 1) {
            label = "anger";
            emotion = Anger;
        }
        else if (counter == 2) {
            label = "disgust";
            emotion = Disgust;
        }
        else if (counter == 3) {
            label = "happy";
            emotion = Happy;
        }
        else if (counter == 4) {
            label = "surprise";
            emotion = Surprise;
        }
        else {
            return;
        }

        //LOG_JOHN() << "Begin Data Parsing";

        pair<vector<string>, vector<string>> dataSets;
        dataSets = getFiles(emotion);

        for (string item : dataSets.first) {
            Mat image = imread(item);
            Mat grey;
            cvtColor(image, grey, COLOR_BGR2GRAY);
            trainingData.push_back(grey);
            trainingLabels.push_back((int)emotion);
        }
        for (string item : dataSets.second) {
            Mat image = imread(item);
            Mat grey;
            cvtColor(image, grey, COLOR_BGR2GRAY);
            predictionData.push_back(grey);
            predictionLabels.push_back((int)emotion);
        }
        //LOG_JOHN() << "Done Data Parsing";

    }
    //done setting up data

    LOG_JOHN() << "Begin Training";
    fishface = face::FisherFaceRecognizer::create();
    fishface->train(trainingData, trainingLabels);
    LOG_JOHN() << "Done Training";

    int correct = 0;
    int incorrect = 0;

    for (unsigned int counter = 0; counter < predictionData.size(); counter++) {
        int predictedLabel = -1;
        double confidence = 0.0;
        valuePercentages = fishface->prediction(predictionData[counter], predictedLabel, confidence);

        LOG_JOHN() << "Values";
        LOG_JOHN() << "Neutral: " + QString::number(valuePercentages.at(0));
        LOG_JOHN() << "Anger: "+ QString::number(valuePercentages.at(1));
        LOG_JOHN() << "Disgust: "+ QString::number(valuePercentages.at(2));
        LOG_JOHN() << "Happy: "+ QString::number(valuePercentages.at(3));
        LOG_JOHN() << "Surprise: "+ QString::number(valuePercentages.at(4));

        if (predictedLabel == predictionLabels[counter]) {
            correct++;
            LOG_JOHN() << QString::number(predictedLabel) + " was correct.";
        }
        else {
            incorrect++;
            LOG_JOHN() << QString::number(predictedLabel) + " vs actual: " + QString::number(predictionLabels[counter]);
        }
    }
    LOG_JOHN() << QString::number((100*correct)/(correct + incorrect)) + "% correct";

    if((100*correct)/(correct + incorrect) > rateThreshold){
        QString path ="C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/Models/" + QString::number((100*correct)/(correct + incorrect))  + "-" + QString::number(coun) + ".yml";
        fishface->save(path.toStdString());
        LOG_JOHN() << "Saved";
        coun++;
    }
    preCheck();

}

void TrainEmotionModel::multiThreadedFinished()
{
    LOG_JOHN() << "Emitted Update";
    progressBar->setText("Finished");
}

void TrainEmotionModel::testModel()
{
    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Choose Model"),QString(), tr("Yaml (*.yml)"));

    fishface = face::FisherFaceRecognizer::load<cv::face::FisherFaceRecognizer>(filename.toStdString());

    int correct = 0;
    int incorrect = 0;
    vector<Mat> testingData;
    vector<int> testingLabels;
    vector<string> files;

    LOG_JOHN() << "1";

    glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/neutral/*.png", files);
    for(string item : files){
        Mat image = imread(item);
        Mat grey;
        cvtColor(image, grey, COLOR_BGR2GRAY);
        testingData.push_back(grey);
        testingLabels.push_back((int)Neutral);
    }
    files.clear();
    glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/anger/*.png", files);
    for(string item : files){
        Mat image = imread(item);
        Mat grey;
        cvtColor(image, grey, COLOR_BGR2GRAY);
        testingData.push_back(grey);
        testingLabels.push_back((int)Anger);
    }
    files.clear();
    glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/disgust/*.png", files);
    for(string item : files){
        Mat image = imread(item);
        Mat grey;
        cvtColor(image, grey, COLOR_BGR2GRAY);
        testingData.push_back(grey);
        testingLabels.push_back((int)Disgust);
    }
    files.clear();
    glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/happy/*.png", files);
    for(string item : files){
        Mat image = imread(item);
        Mat grey;
        cvtColor(image, grey, COLOR_BGR2GRAY);
        testingData.push_back(grey);
        testingLabels.push_back((int)Happy);
    }
    files.clear();
    glob("C:/projects/Shotcut/src/shotcut/opencv/deepnetworkdata/emotion/dataset/surprise/*.png", files);
    for(string item : files){
        Mat image = imread(item);
        Mat grey;
        cvtColor(image, grey, COLOR_BGR2GRAY);
        testingData.push_back(grey);
        testingLabels.push_back((int)Surprise);
    }
    files.clear();
    for (unsigned int counter = 0; counter < testingData.size(); counter++) {
        int predictedLabel = -1;
        double confidence = 0.0;
        valuePercentages = fishface->prediction(testingData[counter], predictedLabel, confidence);

        LOG_JOHN() << "Values";
        LOG_JOHN() << "Neutral: " + QString::number(valuePercentages.at(0));
        LOG_JOHN() << "Anger: "+ QString::number(valuePercentages.at(1));
        LOG_JOHN() << "Disgust: "+ QString::number(valuePercentages.at(2));
        LOG_JOHN() << "Happy: "+ QString::number(valuePercentages.at(3));
        LOG_JOHN() << "Surprise: "+ QString::number(valuePercentages.at(4));

        if (predictedLabel == testingLabels[counter]) {
            correct++;
            LOG_JOHN() << QString::number(predictedLabel) + " was correct.";
        }
        else {
            incorrect++;
            LOG_JOHN() << QString::number(predictedLabel) + " vs actual: " + QString::number(testingLabels[counter]);
        }
    }
    LOG_JOHN() << QString::number((100*correct)/(correct + incorrect)) + "% correct";


}


