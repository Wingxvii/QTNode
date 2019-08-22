#include "emotionkeyframer.h"

EmotionKeyframer::EmotionKeyframer()
{
    //inits
    layout = new QGridLayout;
    window = new QWidget;

    findMethodSelector = new QComboBox();
    labelSelector = new QComboBox();

    thresholdLabel = new QLabel("Threshold");
    threshold = new QLineEdit();

    keyframes = new QListWidget();

    saveData = new QPushButton("Save");
    applyMethod = new QPushButton("Apply Method");

    deleteButton = new QPushButton("Delete");
    clearButton = new QPushButton("Clear");
    openButton = new QPushButton("Open");
    progressBar = new QLabel("Inactive");

    dataOut = std::make_shared<EmotionData>();

    connect(saveData, SIGNAL(clicked(bool)), this, SLOT(saveToFile()));
    connect(applyMethod, SIGNAL(clicked(bool)), this, SLOT(preCheck()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(onDelete()));
    connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(onClear()));
    connect(openButton, SIGNAL(clicked(bool)), this, SLOT(onOpen()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    findMethodSelector->addItem("Pass frames over threshold");
    findMethodSelector->addItem("Pass frames under threshold");
    findMethodSelector->addItem("Pass frames where label is detected");


    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    threshold->setValidator(intPos);

    layout->addWidget(findMethodSelector,1,1);
    layout->addWidget(labelSelector,1,2);
    layout->addWidget(thresholdLabel,2,1);
    layout->addWidget(threshold,2,2);
    layout->addWidget(applyMethod,3,1,1,2);
    layout->addWidget(keyframes,4,1,1,2);
    layout->addWidget(saveData,5,1);
    layout->addWidget(deleteButton,5,2);
    layout->addWidget(clearButton,6,1);
    layout->addWidget(openButton,6,2);
    layout->addWidget(progressBar,7,1);

    window->setLayout(layout);

    buildContextWindow();
}

unsigned int EmotionKeyframer::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch(portType){
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

QtNodes::NodeDataType EmotionKeyframer::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
        return EmotionData().type();
}

std::shared_ptr<QtNodes::NodeData> EmotionKeyframer::outData(QtNodes::PortIndex port)
{
        return dataOut;
}

void EmotionKeyframer::setInData(std::shared_ptr<QtNodes::NodeData> data , int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<EmotionData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //setup labels
            labelSelector->clear();
            if(dataIn && !dataIn->_labels.empty()){
                labelSelector->addItem("All");
                for(std::string label : dataIn->_labels){
                    labelSelector->addItem(QString::fromStdString(label));
                }
            }else{
                labelSelector->addItem("Data Missing");
            }
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;

    }

}

QtNodes::NodeValidationState EmotionKeyframer::validationState() const
{
    return modelValidationState;
}

QString EmotionKeyframer::validationMessage() const
{
    return modelValidationError;
}

void EmotionKeyframer::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &EmotionKeyframer::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void EmotionKeyframer::preCheck()
{

    LOG_JOHN() << "0.1";

    if(threshold->text().size() > 0){
        Threshold = threshold->text().toInt();
    }
    LOG_JOHN() << "0.25";

    if(dataIn && dataIn->isReady && active){
        LOG_JOHN() << "0.3";

        processData();
    }
    LOG_JOHN() << "0.5";

}

void EmotionKeyframer::ShowContextMenu(const QPoint &pos)
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

void EmotionKeyframer::multiThreadedProcess()
{

    LOG_JOHN() << "1";
    //pass through labels
    dataOut->_labels = dataIn->_labels;
    std::vector<std::map<int, double>> tempValuePercentages = std::vector<std::map<int, double>>();
    std::vector<int> frameNumbers;
    //parses data out to dataout
    switch(findMethodSelector->currentIndex()){
    //method of Pass frames over threshold
        case 0:
        LOG_JOHN() << "2";
        if(Threshold == -1){
            progressBar->setText("Error");
            break;
        }

        for(int counter = 0; counter < dataIn->_valuePercentages.size(); counter++){
            std::map<int, double> tempValues = std::map<int, double>();
            //does with all data labels
            LOG_JOHN() << "3";
            if(labelSelector->currentIndex() == 0){
                for(std::pair<int, double> values : dataIn->_valuePercentages[counter]){
                    if(values.second > Threshold){
                        tempValues.emplace(values);
                    }
                }
            }else{  //does with sepific data label
                if(dataIn->_valuePercentages[counter][labelSelector->currentIndex()-1] > Threshold){
                    std::pair<int, double> tempPair = std::pair<int, double>();
                    tempPair.first = labelSelector->currentIndex()-1;
                    tempPair.second = (dataIn->_valuePercentages[counter][labelSelector->currentIndex()-1]);
                    tempValues.emplace(tempPair);
                }
            }
            LOG_JOHN() << "4";
            frameNumbers.push_back(counter);
            tempValuePercentages.push_back(tempValues);
        }
        break;
        //method of Pass frames under threshold
    case 1:
        if(Threshold == -1){
            progressBar->setText("Error");
            break;
        }

        for(int counter = 0; counter < dataIn->_valuePercentages.size(); counter++){
            std::map<int, double> tempValues = std::map<int, double>();
            //does with all data labels
            if(labelSelector->currentIndex() == 0){
                for(std::pair<int, double> values : dataIn->_valuePercentages[counter]){
                    if(values.second < Threshold){
                        tempValues.emplace(values);
                    }
                }
            }else{  //does with sepific data label
                if(dataIn->_valuePercentages[counter][labelSelector->currentIndex()-1] < Threshold){
                    std::pair<int, double> tempPair = std::pair<int, double>();
                    tempPair.first = labelSelector->currentIndex()-1;
                    tempPair.second = (dataIn->_valuePercentages[counter][labelSelector->currentIndex()-1]);
                    tempValues.emplace(tempPair);
                }
            }
            frameNumbers.push_back(counter);
            tempValuePercentages.push_back(tempValues);
        }
        break;
        //frames where label is found
    case 2:
        int selectedLabel = labelSelector->currentIndex()-1;
        if(selectedLabel == -1){
            progressBar->setText("Error");
            break;
        }
        for(int counter = 0; counter < dataIn->_valuePercentages.size(); counter++){
            std::map<int, double> values = dataIn->_valuePercentages[counter];
            double labelValue = dataIn->_valuePercentages[counter].at(selectedLabel);
            bool smallest = true;

            for(std::pair<int, float> compareValue : dataIn->_valuePercentages[counter]){
                if(compareValue.second < labelValue){
                    smallest = false;
                }
            }
            if(smallest){
                tempValuePercentages.push_back(values);
                frameNumbers.push_back(counter);
            }
        }
        break;
    }
    dataOut->_valuePercentages = tempValuePercentages;
    LOG_JOHN() << "5";

    //parse into graph
    for(int counter = 0; counter <frameNumbers.size(); counter++ ){
        for(std::pair<int, double> tempPair: dataOut->_valuePercentages[counter]){
            QString keyframeInfo = "Frame Number: " + QString::number(frameNumbers[counter]);
            keyframeInfo = keyframeInfo + ", Emotion: " + QString::fromStdString(dataOut->_labels[tempPair.first]);
            keyframeInfo = keyframeInfo + ", Value: " + QString::number(tempPair.second);
            keyframes->addItem(keyframeInfo);
        }
    }

    LOG_JOHN() << "6";


}

void EmotionKeyframer::multiThreadedFinished()
{
    progressBar->setText("Finished");
    dataOut->ready();
    emit dataUpdated(0);
}

void EmotionKeyframer::saveToFile()
{

}

void EmotionKeyframer::onDelete()
{

}

void EmotionKeyframer::onClear()
{

}

void EmotionKeyframer::onOpen()
{

}
