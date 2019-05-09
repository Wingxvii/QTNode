#include "calibinfo.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

CalibInfo::CalibInfo(){

    //create the window
    window = new QWidget;

    //instantiate input boxes and labels
    sizeX = new QLineEdit();
    sizeXText = new QLabel("Number of X corners");
    sizeY = new QLineEdit();
    sizeYText = new QLabel("Number of Y corners");
    length = new QLineEdit();
    lengthText = new QLabel("Length of squares (in meters)");

    //instantiate layout
    formLayout = new QFormLayout;

    //instantiate regulation for input
    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    doublePos = new QDoubleValidator();

    //set input regulation
    sizeX->setValidator(intPos);
    sizeY->setValidator(intPos);
    length->setValidator(doublePos);

    //connect input boxes to functions
    connect(sizeX, SIGNAL(textChanged(QString)), this, SLOT(saveXData()));
    connect(sizeY, SIGNAL(textChanged(QString)), this, SLOT(saveYData()));
    connect(length, SIGNAL(textChanged(QString)), this, SLOT(saveLengthData()));

    //attach input and labels to layout
    formLayout->addRow(sizeXText, sizeX);
    formLayout->addRow(sizeYText, sizeY);
    formLayout->addRow(lengthText, length);

    //attach layout to window
    window->setLayout(formLayout);

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


void CalibInfo::saveXData(){
    if(!sizeX->text().isEmpty()){
    data->setSizeXData(sizeX->text().toInt());
    LOG_CAMPBELL() << "X Data Recieved: " << sizeX->text().toInt();
    }
    else{
        data->setSizeXData(0);
    }
}
void CalibInfo::saveYData(){
    if(!sizeY->text().isEmpty()){
    data->setSizeYData(sizeY->text().toInt());
    LOG_CAMPBELL() << "Y Data Recieved: " << sizeY->text().toInt();
    }
    else{
        data->setSizeYData(0);
    }
}
void CalibInfo::saveLengthData(){
    if(!length->text().isEmpty()){
    data->setLengthData(length->text().toFloat());
    LOG_CAMPBELL() << "Length Data Recieved: " << length->text().toFloat();
    }else{
        data->setLengthData(0.0f);
    }
}


