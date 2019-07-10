#include "colorthreshold.h"


//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"


ColorThreshold::ColorThreshold(){

    window = new QWidget;

    HMinEdit = new QSlider(Qt::Horizontal);
    HMaxEdit = new QSlider(Qt::Horizontal);
    SMinEdit = new QSlider(Qt::Horizontal);
    SMaxEdit = new QSlider(Qt::Horizontal);
    VMinEdit = new QSlider(Qt::Horizontal);
    VMaxEdit = new QSlider(Qt::Horizontal);
    progressBar = new QLabel("Inactive");

    HMinEdit->setMinimum(0);
    HMaxEdit->setMinimum(0);
    SMinEdit->setMinimum(0);
    SMaxEdit->setMinimum(0);
    VMinEdit->setMinimum(0);
    VMaxEdit->setMinimum(0);

    HMinEdit->setMaximum(256);
    HMaxEdit->setMaximum(256);
    SMinEdit->setMaximum(256);
    SMaxEdit->setMaximum(256);
    VMinEdit->setMaximum(256);
    VMaxEdit->setMaximum(256);
    \
    HMinText = new QLabel("X Min");
    HMaxText = new QLabel("X Max");
    SMinText = new QLabel("Y Min");
    SMaxText = new QLabel("Y Max");
    VMinText = new QLabel("Z Min");
    VMaxText = new QLabel("Z Max");

    HMinValue = new QLabel("0");
    HMaxValue = new QLabel("0");
    SMinValue = new QLabel("0");
    SMaxValue = new QLabel("0");
    VMinValue = new QLabel("0");
    VMaxValue = new QLabel("0");

    formLayout = new QGridLayout;

    connect(HMinEdit, SIGNAL(sliderReleased()), this, SLOT(saveHData()));
    connect(HMaxEdit, SIGNAL(sliderReleased()), this, SLOT(saveHData()));
    connect(SMinEdit, SIGNAL(sliderReleased()), this, SLOT(saveSData()));
    connect(SMaxEdit, SIGNAL(sliderReleased()), this, SLOT(saveSData()));
    connect(VMinEdit, SIGNAL(sliderReleased()), this, SLOT(saveVData()));
    connect(VMaxEdit, SIGNAL(sliderReleased()), this, SLOT(saveVData()));
    connect(HMinEdit, SIGNAL(valueChanged(int)), this, SLOT(updateHData()));
    connect(HMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(updateHData()));
    connect(SMinEdit, SIGNAL(valueChanged(int)), this, SLOT(updateSData()));
    connect(SMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(updateSData()));
    connect(VMinEdit, SIGNAL(valueChanged(int)), this, SLOT(updateVData()));
    connect(VMaxEdit, SIGNAL(valueChanged(int)), this, SLOT(updateVData()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));


    formLayout->addWidget(HMinText,1,1);
    formLayout->addWidget(HMaxText,2,1);
    formLayout->addWidget(SMinText,3,1);
    formLayout->addWidget(SMaxText,4,1);
    formLayout->addWidget(VMinText,5,1);
    formLayout->addWidget(VMaxText,6,1);

    formLayout->addWidget(HMinEdit,1,2);
    formLayout->addWidget(HMaxEdit,2,2);
    formLayout->addWidget(SMinEdit,3,2);
    formLayout->addWidget(SMaxEdit,4,2);
    formLayout->addWidget(VMinEdit,5,2);
    formLayout->addWidget(VMaxEdit,6,2);

    formLayout->addWidget(HMinValue,1,3);
    formLayout->addWidget(HMaxValue,2,3);
    formLayout->addWidget(SMinValue,3,3);
    formLayout->addWidget(SMaxValue,4,3);
    formLayout->addWidget(VMinValue,5,3);
    formLayout->addWidget(VMaxValue,6,3);


    formLayout->addWidget(progressBar);
    window->setLayout(formLayout);

    outVideo = std::make_shared<VideoGraphData>();
    buildContextWindow();

}

unsigned int ColorThreshold::nPorts(QtNodes::PortType portType)const
{
    return 1;
}

std::shared_ptr<NodeData> ColorThreshold::outData(PortIndex port){
    return outVideo;
}

void ColorThreshold::setInData(std::shared_ptr<NodeData> data, int location){
    inVideo = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(inVideo){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            //data was found
            preCheck();
        }
        else{
            modelValidationState = NodeValidationState::Warning;
            modelValidationError = QStringLiteral("Missing or incorrect inputs");
            LOG_JOHN() << "Video data was not found";
        }

}
NodeDataType ColorThreshold::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

NodeValidationState ColorThreshold::validationState() const
{
    return modelValidationState;
}

QString ColorThreshold::validationMessage() const
{
    return modelValidationError;
}

void ColorThreshold::multiThreadedProcess(){
    std::vector<cv::Mat> tempVid;
        cv::Mat temp;
        for(int x = 0; x < inVideo->data().size(); x++){
            cv::inRange(inVideo->data().at(x), cv::Scalar(HMin, SMin, VMin), cv::Scalar(HMax, SMax, VMax), temp);
            tempVid.push_back(temp.clone());
            //LOG_JOHN() << "Applied " + QString::number(x);
        }
        outVideo->_video = tempVid;
}

void ColorThreshold::multiThreadedFinished()
{
    progressBar->setText("Finished");
    outVideo->ready();
    emit dataUpdated(0);

}

void ColorThreshold::saveHData(){
    LOG_JOHN() << "Saved " + QString::number(HMin) + "," + QString::number(HMax);
    HMin = HMinEdit->value();
    HMax = HMaxEdit->value();
    preCheck();
}
void ColorThreshold::saveSData(){
    LOG_JOHN() << "Saved " + QString::number(SMin) + "," + QString::number(SMax);
    SMin = SMinEdit->value();
    SMax = SMaxEdit->value();
    preCheck();

}
void ColorThreshold::saveVData(){
    LOG_JOHN() << "Saved " + QString::number(VMin) + "," + QString::number(VMax);
    VMin = VMinEdit->value();
    VMax = VMaxEdit->value();
    preCheck();
}

void ColorThreshold::updateHData()
{
    HMinValue->setText(QString().number(HMinEdit->value()));
    HMaxValue->setText(QString().number(HMaxEdit->value()));
}


void ColorThreshold::updateSData()
{
    SMinValue->setText(QString().number(SMinEdit->value()));
    SMaxValue->setText(QString().number(SMaxEdit->value()));

}

void ColorThreshold::updateVData()
{
    VMinValue->setText(QString().number(VMinEdit->value()));
    VMaxValue->setText(QString().number(VMaxEdit->value()));

}

QJsonObject ColorThreshold::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    dataJson["xMin"] = HMin;
    dataJson["xMax"] = HMax;
    dataJson["yMin"] = SMin;
    dataJson["yMax"] = SMax;
    dataJson["zMin"] = VMin;
    dataJson["zMax"] = VMax;

    return dataJson;

}

void ColorThreshold::restore(const QJsonObject & json)
{
    if(json.contains("xMin")){
        HMinEdit->setValue(json["xMin"].toInt());
    }
    if(json.contains("xMax")){
        HMaxEdit->setValue(json["xMax"].toInt());
    }
    if(json.contains("yMin")){
        SMinEdit->setValue(json["yMin"].toInt());
    }
    if(json.contains("yMax")){
        SMaxEdit->setValue(json["yMax"].toInt());
    }
    if(json.contains("zMin")){
        VMinEdit->setValue(json["zMin"].toInt());
    }
    if(json.contains("zMax")){
        VMaxEdit->setValue(json["zMax"].toInt());
    }
    saveHData();
    saveSData();
    saveVData();

    preCheck();

}

void ColorThreshold::processData()
{
    //setup progress bar parameters
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &ColorThreshold::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void ColorThreshold::preCheck()
{

    if(inVideo && inVideo->isReady && active){
        processData();
    }else{
        if(outVideo){outVideo->unready();}
    }

}

void ColorThreshold::ShowContextMenu(const QPoint &pos)
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
