#include "savedata.h"
#include <QInputDialog>


SaveData::SaveData()
{
    window = new QWidget;
    layout = new QGridLayout;
    dataIndexDisplay = new QLabel("Data Pending...");
    deleteButton = new QPushButton("Delete Data");
    saveButton = new QPushButton("Save Data");
    cloneButton = new QPushButton("Clone Data");
    displayButton = new QPushButton("Display Data");

    nameIndexInput = new QLineEdit("");
    typeIndexInput = new QComboBox();
    setUpTypes();

    layout->addWidget(dataIndexDisplay,1,1,1,2);
    layout->addWidget(displayButton,2,1);
    layout->addWidget(saveButton,2,2);
    layout->addWidget(deleteButton,3,1);
    layout->addWidget(cloneButton,3,2);
    layout->addWidget(nameIndexInput,1,3);
    layout->addWidget(typeIndexInput,1,4);

    window->setLayout(layout);
    window->setVisible(false);
    window->setWindowTitle("Data Actions");

    openAction = new QAction(tr("&Open Actions Menu"), this);
    openAction->setStatusTip("Open Data Saving Window");
    connect(openAction, SIGNAL(triggered()), this, SLOT(openSaveWindow()));

    connect(displayButton , SIGNAL(clicked(bool)), this, SLOT(onDisplay()));
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(onSave()));
    connect(cloneButton, SIGNAL(clicked(bool)), this, SLOT(onClone()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(onDelete()));
    connect(nameIndexInput, SIGNAL(textEdited(const QString)),this,SLOT(updateLabel()));
    connect(typeIndexInput, SIGNAL(activated(int)), this, SLOT(updateLabel()));

}

void SaveData::setUpTypes()
{
    typeIndexInput->addItem("Calib");       //0
    typeIndexInput->addItem("Image");       //1
    typeIndexInput->addItem("Point");       //2
    typeIndexInput->addItem("Points");      //3
    typeIndexInput->addItem("Video");       //4
    typeIndexInput->addItem("Int");         //5
    typeIndexInput->addItem("Float");       //6
    typeIndexInput->addItem("String");      //7
    typeIndexInput->addItem("Bool");        //8
}

void SaveData::openSaveWindow()
{
    typeIndex = -1;
    nameIndex = "";
    updateLabel();

    nameIndexInput->setVisible(true);
    typeIndexInput->setVisible(true);
    //popup of this widget window
    window->setVisible(true);

}

void SaveData::openSaveWindow(QString nameIndexIn, int typeIndexIn)
{
    nameIndexInput->setVisible(false);
    typeIndexInput->setVisible(false);
    typeIndex = typeIndexIn;
    nameIndex = nameIndexIn;

    LOG_JOHN() << typeIndex;
    LOG_JOHN() << nameIndex;

    updateLabel();
    window->setVisible(true);

}

void SaveData::onDelete()
{
    LOG_JOHN() << "Delete pressed";
    LinkManager::instance()->deleteData(nameIndex, typeIndex);

    //clear data
    typeIndex = -1;
    nameIndex = "";

    window->setVisible(false);
}

void SaveData::onSave()
{
    LOG_JOHN() << "Save pressed";
    LinkManager::instance()->saveData(nameIndex, typeIndex);
    window->setVisible(false);
}

void SaveData::onClone()
{
    LOG_JOHN() << "Clone pressed";
    LinkManager::instance()->cloneData(nameIndex, typeIndex);
    window->setVisible(false);
}

void SaveData::onDisplay()
{
    LOG_JOHN() << "Display pressed";
    LinkManager::instance()->displayData(nameIndex, typeIndex);

}

void SaveData::updateLabel()
{
    if(nameIndexInput->isVisible()){
        nameIndex = nameIndexInput->text();
    }
    if(typeIndexInput->isVisible()){
        typeIndex = typeIndexInput->currentIndex();
    }


    QString labelStr;

    switch(typeIndex){
    case 0:
        if(LinkManager::instance()->getCalibData(nameIndex))
        {
            labelStr = "Calib Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 1:
        if(LinkManager::instance()->getImageData(nameIndex))
        {
            labelStr = "Image Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 2:
        if(LinkManager::instance()->getPointData(nameIndex))
        {
            labelStr = "Point Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 3:
        if(LinkManager::instance()->getPointsData(nameIndex))
        {
            labelStr = "Points Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 4:
        if(LinkManager::instance()->getVideoData(nameIndex))
        {
            labelStr = "Video Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 5:
        if(LinkManager::instance()->getIntData(nameIndex))
        {
            labelStr = "Int Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 6:
        if(LinkManager::instance()->getFloatData(nameIndex))
        {
            labelStr = "Float Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 7:
        if(LinkManager::instance()->getStringData(nameIndex) != NULL)
        {
            labelStr = "String Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    case 8:
        if(LinkManager::instance()->getBoolData(nameIndex))
        {
            labelStr = "Bool Data at [:";
            labelStr.append(nameIndex);
            labelStr.append(":]");
        }else{
            labelStr = "No Data";
        }
        dataIndexDisplay->setText(labelStr);
        break;
    default:
        dataIndexDisplay->setText("Type Not Selected");
        break;
    }
}

