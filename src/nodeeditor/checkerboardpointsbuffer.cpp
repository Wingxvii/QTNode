#include "checkerboardpointsbuffer.h"

CheckerboardPointsBuffer::CheckerboardPointsBuffer(){

    window = new QWidget;

    addButton = new QPushButton("Pull");
    deleteButton = new QPushButton("Delete");
    dataList = new QListWidget();
    layout = new QGridLayout();

    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addData()));
    connect(deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteData()));

    layout->addWidget(addButton, 1,2);
    layout->addWidget(dataList, 1,1);
    layout->addWidget(deleteButton,2,2);

    window->setLayout(layout);
}
CheckerboardPointsBuffer::~CheckerboardPointsBuffer(){}

unsigned int CheckerboardPointsBuffer::nPorts(QtNodes::PortType portType)const
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

std::shared_ptr<NodeData> CheckerboardPointsBuffer::outData(PortIndex port)
{
    switch(port){
    case 0:
        if(pointsOut){
            return pointsOut;
        }
        break;
    }
    return NULL;

}


void CheckerboardPointsBuffer::setInData(std::shared_ptr<NodeData> data, int location){
    pointIn = std::dynamic_pointer_cast<PointData>(data);
    pointsOut = std::make_shared<PointsData>();
    if(pointIn){

        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
        //data was found
    }
    else{
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        LOG_CAMPBELL() << "Points in data was not found";
    }

}

NodeDataType CheckerboardPointsBuffer::dataType(PortType portType, PortIndex portIndex) const{
    if(portType == PortType::In){
        return PointData().type();
    }
    return PointsData().type();

}


NodeValidationState CheckerboardPointsBuffer::validationState() const
{
    return modelValidationState;
}

QString CheckerboardPointsBuffer::validationMessage() const
{
    return modelValidationError;
}

void CheckerboardPointsBuffer::deleteData()
{
    if(dataList->currentItem()){
        pointsOut->removeData(dataList->currentRow());
        delete dataList->takeItem(dataList->currentRow());
    }

}
void CheckerboardPointsBuffer::addData()
{
    numberOfPoints++;
    if(!pointIn->data().empty()){
        pointsOut->addData(pointIn->data());
        QString num = QString::number(numberOfPoints);
        QString prefix = "Points #";
        prefix.append(num);
        dataList->addItem(prefix);
    }

}
