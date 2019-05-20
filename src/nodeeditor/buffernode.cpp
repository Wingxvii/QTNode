#include "buffernode.h"
#include "QAction"

BufferNode::BufferNode(){
    //init
    layout = new QGridLayout;
    window = new QWidget;

    activateButton = new QPushButton("Activate");
    deactivateButton = new QPushButton("Deactivate");
    status = new QLabel("Active");

    connect(activateButton, SIGNAL(clicked()), this, SLOT(activate()));
    connect(deactivateButton, SIGNAL(clicked()), this, SLOT(deactivate()));

    layout->addWidget(status, 1,1);
    layout->addWidget(activateButton,2,1);
    layout->addWidget(deactivateButton,3,1);

    window->setLayout(layout);

    buildContextWindow();

}
unsigned int BufferNode::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 3;

    switch(portType){
    case PortType::In:
        result = 3;
        break;
    case PortType::Out:
        result = 3;
        break;
    default:
        break;
    }
    return result;
}

QtNodes::NodeDataType BufferNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return NodeData().type();
}

std::shared_ptr<NodeData> BufferNode::outData(PortIndex port)
{
    switch(port){
    case 0:
        if(outPort1){
            return outPort1;
        }
        break;
    case 1:
        if(outPort2){
            return outPort2;
        }
        break;
    case 2:
        if(outPort3){
            return outPort3;
        }
        break;

    return NULL;
    }
}

void BufferNode::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{

    switch(location){
    case 0:
        inPort1 = data;
        preCheck();
        break;
    case 1:
        inPort2 = data;
        preCheck();
        break;
    case 2:
        inPort3 = data;
        preCheck();
        break;
    }
}
QtNodes::NodeValidationState BufferNode::validationState() const
{
    return modelValidationState;
}

QString BufferNode::validationMessage() const
{
    return modelValidationError;
}

void BufferNode::processData()
{
    if(inPort1){
        outPort1 = inPort1;
        outPort1->ready();
    }
    if(inPort2){
        outPort2 = inPort2;
        outPort2->ready();
    }
    if(inPort3){
        outPort3 = inPort3;
        outPort3->ready();
    }
}

void BufferNode::preCheck()
{
    if(active){
        processData();
        emit dataUpdated(0);
    }else{
        if(outPort1){outPort1->unready();}
        if(outPort2){outPort2->unready();}
        if(outPort3){outPort3->unready();}
    }
}

void BufferNode::activate()
{
    active = true;
    status->setText("Active");
    preCheck();
}

void BufferNode::deactivate()
{
    active = false;
    status->setText("Inactive");
}

void BufferNode::ShowContextMenu(const QPoint &pos)
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
