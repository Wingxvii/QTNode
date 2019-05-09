#include "autoframeiterator.h"

//opencv
#include "opencv2\core.hpp"


AutoFrameIterator::AutoFrameIterator()
{
    //init everything
    window = new QWidget;
    layout = new QGridLayout;

    totalFramesDisplay = new QLabel(QString::number(totalFrames));
    totalFramesLabel = new QLabel("Total Frames: ");
    startLabel = new QLabel("Start Frame: ");
    startFrameInput = new QLineEdit();
    endLabel = new QLabel("End Frame: ");
    endFrameInput = new QLineEdit();
    byPassLabel = new QLabel("Bypass: ");
    byPassInput = new QLineEdit();
    maxSampleLabel = new QLabel("Max Samples: ");
    maxSamplesInput = new QLineEdit();
    progressBar = new QProgressBar();
    projectedSamplesLabel = new QLabel("Estimated number of samples: ");
    projectedSamplesDisplay = new QLabel(QString::number(projectedSamples));
    startButton = new QPushButton("Start Iteration");

    intPos = new QRegExpValidator(QRegExp("\\d*"), this);

    //add input regulation
    startFrameInput->setValidator(intPos);
    endFrameInput->setValidator(intPos);
    byPassInput->setValidator(intPos);
    maxSamplesInput->setValidator(intPos);

    //connect functions to slots
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startIteration()));
    connect(startFrameInput, SIGNAL(textChanged(Qstring)), this, SLOT(calcValues()));
    connect(endFrameInput, SIGNAL(textChanged(Qstring)), this, SLOT(calcValues()));
    connect(byPassInput, SIGNAL(textChanged(Qstring)), this, SLOT(calcValues()));
    connect(maxSamplesInput, SIGNAL(textChanged(Qstring)), this, SLOT(calcValues()));

    //build layout
    layout->addWidget(totalFramesLabel, 1,1);
    layout->addWidget(totalFramesDisplay, 1,2);
    layout->addWidget(startLabel,2,1);
    layout->addWidget(startFrameInput,2,2);
    layout->addWidget(endLabel, 2,3);
    layout->addWidget(endFrameInput, 2, 4);
    layout->addWidget(byPassLabel, 3,1);
    layout->addWidget(byPassInput,3,2);
    layout->addWidget(maxSampleLabel,3,3);
    layout->addWidget(maxSamplesInput,3,4);
    layout->addWidget(progressBar,4,1);
    layout->addWidget(projectedSamplesLabel,5,1);
    layout->addWidget(projectedSamplesDisplay,5,2);
    layout->addWidget(startButton,6,1);
    //set layout into window
    window->setLayout(layout);
}

unsigned int AutoFrameIterator::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType AutoFrameIterator::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if(portType == PortType::In){
        return VideoGraphData().type();
    }
    return ImageVector().type();
}

std::shared_ptr<NodeData> AutoFrameIterator::outData(PortIndex port)
{
    switch(port){
    case 0:
        if(imagesOut){
            return imagesOut;
        }
        break;
    return NULL;
    }
}

void AutoFrameIterator::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{

    switch(location){
    case 0:
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            imagesOut = std::make_shared<ImageVector>();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState AutoFrameIterator::validationState() const
{
    return modelValidationState;
}

QString AutoFrameIterator::validationMessage() const
{
    return modelValidationError;
}

void AutoFrameIterator::startIteration()
{

}

void AutoFrameIterator::calcValues()
{
    //update values
}
