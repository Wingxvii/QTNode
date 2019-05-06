#include "makearuco.h"

#include <opencv2/highgui.hpp>


MakeAruco::MakeAruco(){
    //choose file button
    filebutton = new QPushButton("Save Dir");
    connect(filebutton, SIGNAL(clicked(bool)), this, SLOT(createArucoMarker()));
    //size choice combo box
    sizeBox = new QComboBox();
    sizeBox->addItem("4x4");
    sizeBox->addItem("5x5");
    sizeBox->addItem("6x6");
    sizeBox->addItem("7x7");
    connect(sizeBox, SIGNAL(currentIndexChanged(int index)), this, SLOT(setSize(index)));

    //number choice combo box
    numberBox = new QComboBox();
    numberBox->addItem("50");
    numberBox->addItem("100");
    numberBox->addItem("250");
    numberBox->addItem("1000");
    connect(sizeBox, SIGNAL(currentIndexChanged(int index)), this, SLOT(setNumber(index)));

    //resolution choice combo box
    resolutionBox = new QComboBox();
    resolutionBox->addItem("100");
    resolutionBox->addItem("250");
    resolutionBox->addItem("500");
    resolutionBox->addItem("1000");
    connect(sizeBox, SIGNAL(currentIndexChanged(int index)), this, SLOT(setResolution(index)));


    //set layout
    layout = new QVBoxLayout();
    layout->addWidget(sizeBox);
    layout->addWidget(numberBox);
    layout->addWidget(resolutionBox);
    layout->addWidget(filebutton);

    //add to container
    container = new QWidget();
    container->setLayout(layout);


}

MakeAruco::~MakeAruco(){
    delete filebutton;
    delete container;
    delete layout;
    delete sizeBox;
    delete numberBox;
    delete resolutionBox;

}

void MakeAruco::setSize(int index){
    switch (index) {
    case 1:
        size = 5;
        break;
    case 2:
        size = 6;
        break;
    case 3:
        size = 7;
        break;
    default:
        size = 4;
        break;

    }

}

void MakeAruco::setNum(int index){
    switch (index) {
    case 0:
        number = 50;
        break;
    case 1:
        number = 100;
        break;
    case 2:
        number = 250;
        break;
    case 3:
        number = 1000;
        break;
    default:
        number = 50;
        break;
    }

}

void MakeAruco::setPixle(int index){
    switch (index) {
    case 0:
        pixelSize = 100;
        break;
    case 1:
        pixelSize = 250;
        break;
    case 2:
        pixelSize = 500;
        break;
    case 3:
        pixelSize = 1000;
        break;
    default:
        number = 100;
        break;
    }

}

void MakeAruco::createArucoMarker(){
    cv::Mat output;
      cv::Ptr<cv::aruco::Dictionary> marker;

       QString fileDir = QFileDialog::getExistingDirectory(container, tr("Open Directory"));

       //fetches aruco markers based on specifications
       switch (size) {
       case 5:

           if(number == 100){

               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_100);
           }else if(number == 250){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_250);
           }else if(number == 1000){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_1000);
           }else{
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50);
           }
           break;
       case 6:

           if(number == 100){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_100);
           }else if(number == 250){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
           }else if(number == 1000){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_1000);
           }else{
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_50);
           }
           break;

       case 7:

           if(number == 100){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_100);
           }else if(number == 250){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_250);
           }else if(number == 1000){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_1000);
           }else{
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_50);
           }
           break;

       default:

           if(number == 100){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);
           }else if(number == 250){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
           }else if(number == 1000){
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_1000);
           }else{
               marker = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
           }

           break;
       }

       //iterate through all markers and write them to selected directory
       for(int i = 0; i < number; i++){

           cv::aruco::drawMarker(marker, i, pixelSize * size, output, 1);

           std::ostringstream convert;

           convert << fileDir.toStdString() << size << "_MAKRER_" << i << ".png";

           cv::imwrite(convert.str(), output);
       }

}

NodeValidationState MakeAruco::validationState() const
{
    return modelValidationState;
}

QString MakeAruco::validationMessage() const
{
    return modelValidationError;
}
