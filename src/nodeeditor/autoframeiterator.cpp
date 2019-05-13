#include "autoframeiterator.h"

//opencv


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
    progressBar = new QProgressBar();
    projectedSamplesLabel = new QLabel("Estimated number of samples: ");
    projectedSamplesDisplay = new QLabel(QString::number(projectedSamples));
    startButton = new QPushButton("Start Iteration");

    //init out port
    imagesOut = std::make_shared<VideoGraphData>();

    //signed int
    intPos = new QRegExpValidator(QRegExp("-?\\d*"), this);

    //add input regulation
    startFrameInput->setValidator(intPos);
    endFrameInput->setValidator(intPos);
    byPassInput->setValidator(intPos);

    //connect functions to slots
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startIteration()));
    connect(startFrameInput, SIGNAL(textChanged(QString)), this, SLOT(calcValues()));
    connect(endFrameInput, SIGNAL(textChanged(QString)), this, SLOT(calcValues()));
    connect(byPassInput, SIGNAL(textChanged(QString)), this, SLOT(calcValues()));

    //build layout
    layout->addWidget(totalFramesLabel, 1,1);
    layout->addWidget(totalFramesDisplay, 1,2);
    layout->addWidget(startLabel,2,1);
    layout->addWidget(startFrameInput,2,2);
    layout->addWidget(endLabel, 2,3);
    layout->addWidget(endFrameInput, 2, 4);
    layout->addWidget(byPassLabel, 3,1);
    layout->addWidget(byPassInput,3,2);
    layout->addWidget(progressBar,4,1,1,4);
    layout->addWidget(startButton,5,1,1,2);
    layout->addWidget(projectedSamplesLabel,5,3);
    layout->addWidget(projectedSamplesDisplay,5,4);
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
    return VideoGraphData().type();
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
            imagesOut = std::make_shared<VideoGraphData>();
            calcValues(); //update values
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
    if(videoIn){
        std::vector<cv::Mat> temp;

        //handle endframe -1
        int _endFrame = endFrame;
        if(endFrame == -1){
            _endFrame = totalFrames;
        }

        //setup progress bar parameters
        progressBar->setMinimum(startFrame);
        progressBar->setMaximum(_endFrame);

        //iterate     //MULTITHREAD THIS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        for(int counter = startFrame; counter < _endFrame; counter += byPass){
            temp.push_back(videoIn->data().at(counter));
            progressBar->setValue(counter);
        }
        progressBar->setValue(_endFrame);
        imagesOut->_video = temp;
    }
}

void AutoFrameIterator::calcValues()
{
    if(videoIn){                                    //total frames
        totalFrames = videoIn->data().size();
        //set label
        totalFramesDisplay->setText(QString::number(totalFrames));
    }
    if(!startFrameInput->text().isEmpty()){         //start frame
        startFrame = startFrameInput->text().toInt();
    }
    if(!endFrameInput->text().isEmpty()){           //end frame
        endFrame = endFrameInput->text().toInt();
    }
    if(!byPassInput->text().isEmpty()){             //bypass amount
        byPass = byPassInput->text().toInt();
    }

    //calculate projected samples
    projectedSamples = totalFrames;

    if(startFrame > 0){
        projectedSamples -= startFrame;
    }
    if(endFrame < totalFrames && endFrame != -1){
        projectedSamples -= totalFrames - endFrame;
    };

    //logic checks
    if(projectedSamples < 1){
        projectedSamples = 0;
    }

    //divide by bypass
    if(byPass > 0){
        projectedSamples /= byPass;
    }
    //update display
    projectedSamplesDisplay->setText(QString::number(projectedSamples));

    LOG_JOHN() << "Values Updated";

}
