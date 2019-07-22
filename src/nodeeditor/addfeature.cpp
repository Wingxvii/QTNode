#include "addfeature.h"

AddFeature::AddFeature()
{
    window = new QWidget;
    layout = new QGridLayout;

    enterButton = new QPushButton("Enter");
    cancelButton = new QPushButton("Cancel");
    inputStart = new QLabel("Coordinates:(");
    inputMiddle = new QLabel(",");
    inputEnd = new QLabel(")");
    xInput = new QLineEdit();
    yInput = new QLineEdit();
    nameInputLabel = new QLabel("Name:");
    nameInput = new QLineEdit();

    layout->addWidget(inputStart,1,1);
    layout->addWidget(xInput,1,2);
    layout->addWidget(inputMiddle,1,3);
    layout->addWidget(yInput,1,4);
    layout->addWidget(inputEnd,1,5);
    layout->addWidget(nameInputLabel, 2,1,1,2);
    layout->addWidget(nameInput, 2,3,1,2);

    layout->addWidget(enterButton,3,2,1,2);
    layout->addWidget(cancelButton,3,4,1,2);

    window->setLayout(layout);
    window->setVisible(false);
    window->setWindowTitle("Add New Point");

    intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    xInput->setValidator(intPos);
    yInput->setValidator(intPos);

    connect(cancelButton , SIGNAL(clicked(bool)), this, SLOT(onExit()));
    connect(enterButton , SIGNAL(clicked(bool)), this, SLOT(onEnter()));

}

void AddFeature::openWindow()
{
    edit = false;
    window->setVisible(true);
}

void AddFeature::onEnter()
{
    if(!xInput->text().isEmpty()){
        x = xInput->text().toInt();
    }
    if(!yInput->text().isEmpty()){
        y = yInput->text().toInt();
    }
    if(!nameInput->text().isEmpty()){
        name = nameInput->text();
    }

    if(x != -1 && y != -1 && name != ""){
        if(!edit){
            emit sendPoint(x,y, name);
        }else{
            emit sendEdit(row, x,y,name);
        }
        onExit();
    }
}

void AddFeature::onExit()
{
    window->setVisible(false);
}

void AddFeature::onEdit(int row, int x, int y, QString name)
{
    this->row = row;
    xInput->setText(QString::number(x));
    yInput->setText(QString::number(y));
    nameInput->setText(name);

    edit = true;
    window->setVisible(true);

}
