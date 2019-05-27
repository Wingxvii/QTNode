#include "calibinfo.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

CalibInfo::CalibInfo(){

    //instantiate input boxes and labels
    sizeX = new QLineEdit();
    sizeXText = new QLabel("Number of X corners");
    sizeY = new QLineEdit();
    sizeYText = new QLabel("Number of Y corners");
    length = new QLineEdit();
    lengthText = new QLabel("Length of squares (in meters)");

    //instantiate layout
    formLayout = new QFormLayout;
    window =  new QWidget;

    //instantiate regulation for input
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    doublePos = new QDoubleValidator();

    //set input regulation
    sizeX->setValidator(intPos);
    sizeY->setValidator(intPos);
    length->setValidator(doublePos);

    //connect input boxes to functions
    connect(sizeX, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(sizeY, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(length, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    //attach input and labels to layout
    formLayout->addRow(sizeXText, sizeX);
    formLayout->addRow(sizeYText, sizeY);
    formLayout->addRow(lengthText, length);

    //attach layout to window
    window->setLayout(formLayout);

    //init context window
    buildContextWindow();

    data = std::make_shared<CalibData>();
}

unsigned int CalibInfo::nPorts(QtNodes::PortType portType)const
{
    unsigned int result = 0;

    switch(portType){
    case PortType::In:
        result = 0;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

std::shared_ptr<NodeData> CalibInfo::outData(PortIndex port){
    return data;
}

NodeDataType CalibInfo::dataType(PortType portType, PortIndex portIndex) const
{
    return CalibData().type();
}

void CalibInfo::preCheck(){
    if(!sizeX->text().isEmpty()){
    data->setSizeXData(sizeX->text().toInt());
    LOG_JOHN() << "X Data Recieved: " << sizeX->text().toInt();
    }
    else{
        data->setSizeXData(0);
    }
    if(!sizeY->text().isEmpty()){
    data->setSizeYData(sizeY->text().toInt());
    LOG_JOHN() << "Y Data Recieved: " << sizeX->text().toInt();
    }
    else{
        data->setSizeYData(0);
    }
    if(!length->text().isEmpty()){
    data->setLengthData(length->text().toFloat());
    }else{
        data->setLengthData(0.0f);
    }

    if(active){
        processData();
    }

}


void CalibInfo::processData(){
    data->checkReady();
    emit dataUpdated(0);
}

void CalibInfo::ShowContextMenu(const QPoint &pos)
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

QJsonObject CalibInfo::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    dataJson["sizeX"] = sizeX->text();
    dataJson["sizeY"] = sizeY->text();
    dataJson["length"] = length->text();

    return dataJson;

}

void CalibInfo::restore(const QJsonObject &json)
{
    if(json.contains("sizeX")){
        sizeX->setText(json["sizeX"].toString());
    }
    if(json.contains("sizeY")){
        sizeY->setText(json["sizeY"].toString());
    }
    if(json.contains("length")){
        length->setText(json["length"].toString());
    }

    preCheck();
}




