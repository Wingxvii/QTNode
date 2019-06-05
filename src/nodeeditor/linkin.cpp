#include "linkin.h"


CalibLinkIn::CalibLinkIn()
{
    layout = new QGridLayout;
    window = new QWidget;

    //inits
    statusLabel = new QLabel("Default");
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

unsigned int CalibLinkIn::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;
    switch(portType){
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 0;
        break;
    default:
        break;

    }
    return result;
}

QtNodes::NodeDataType CalibLinkIn::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return CalibData().type();
}

void CalibLinkIn::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<CalibData>(data);
        if(dataIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState CalibLinkIn::validationState() const
{
    return modelValidationState;
}

QString CalibLinkIn::validationMessage() const
{
    return modelValidationError;
}

void CalibLinkIn::processData()
{
    std::shared_ptr<CalibData> calibOut;
    calibOut = std::dynamic_pointer_cast<CalibData>(dataIn);
    LinkManager::instance()->sendData(calibOut ,index);
}

void CalibLinkIn::preCheck()
{
    index = indexInput->text();

    if(LinkManager::instance()->getCalibData(index)){
        statusLabel->setText("Data Slot In Use");
    }else{
        statusLabel->setText("Data Slot Empty");
    }

    LOG_JOHN() << "Checkers";
    if(this->active && dataIn &&dataIn->isReady){
        LOG_JOHN() << "Thing works";
        processData();
        statusLabel->setText("Data Slot Updated");
    }
}

void CalibLinkIn::ShowContextMenu(const QPoint &pos)
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

