#include "linkout.h"

CalibLinkOut::CalibLinkOut()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel();
    indexLabel = new QLabel("Select Data Index: ");
    indexInput = new QLineEdit();
    indexInput->setText("0");


    //connect
    connect(indexInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

    layout->addWidget(indexLabel, 1,1);
    layout->addWidget(indexInput, 1,2);
    layout->addWidget(statusLabel,2,1);
    window->setLayout(layout);

    //ensure this is deactivated
    this->deactivate();

    buildContextWindow();
    preCheck();
}


unsigned int CalibLinkOut::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
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

QtNodes::NodeDataType CalibLinkOut::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return CalibData().type();
}

std::shared_ptr<QtNodes::NodeData> CalibLinkOut::outData(QtNodes::PortIndex port)
{
    switch(port){
    case 0:
    if(dataOut){
        return dataOut;
    }
        break;
    return NULL;
    }
}

void CalibLinkOut::processData()
{
    dataOut = LinkManager::instance()->getCalibData(index);
}

void CalibLinkOut::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getCalibData(index)){
        statusLabel->setText("Data Active");
    }else{
        statusLabel->setText("Data Slot Empty");
    }
    LOG_JOHN() << "Checkers";
    if(this->active && LinkManager::instance()->getCalibData(index)){
        LOG_JOHN() << "Thing works";
        processData();
        emit dataUpdated(0);
    }

}

void CalibLinkOut::ShowContextMenu(const QPoint &pos)
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

