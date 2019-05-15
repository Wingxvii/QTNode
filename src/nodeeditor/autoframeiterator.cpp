#include "autoframeiterator.h"
#include "QMenu"
#include "QAction"
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
   // startButton = new QPushButton("Start Iteration");

    //init out port
    imagesOut = std::make_shared<VideoGraphData>();

    //signed int
    intPos = new QRegExpValidator(QRegExp("-?\\d*"), this);

    //add input regulation
    startFrameInput->setValidator(intPos);
    endFrameInput->setValidator(intPos);
    byPassInput->setValidator(intPos);

    //connect functions to slots
    //connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startIteration()));
    connect(startFrameInput, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));
    connect(endFrameInput, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));
    connect(byPassInput, SIGNAL(textChanged(QString)), this, SLOT(updateUI()));
    connect(startFrameInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(endFrameInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));
    connect(byPassInput, SIGNAL(textChanged(QString)), this, SLOT(preCheck()));

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
    //layout->addWidget(startButton,5,1,1,2);
    layout->addWidget(projectedSamplesLabel,5,1);
    layout->addWidget(projectedSamplesDisplay,5,2);
    //set layout into window
    window->setLayout(layout);


    //context window
    window->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(window, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));


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
            updateUI(); //update values
            preCheck();
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

void AutoFrameIterator::processData()
{
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
    imagesOut->ready();
}

void AutoFrameIterator::updateUI()
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
    if(endFrame < totalFrames && endFrame > startFrame && endFrame != -1){
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

void AutoFrameIterator::preCheck(){
    //use this to check if ports are ready

    if(startFrame && endFrame && byPass && byPass != 0){
        isReady = true;
    }

    if(videoIn && videoIn->isReady && isReady){
        processData();
        emit dataUpdated(0);
        updateUI();
    }else{
        if(imagesOut){imagesOut->unready();}

    }

}

void AutoFrameIterator::ShowContextMenu(const QPoint &pos)
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

void AutoFrameIterator::activate()
{
    active = true;
    startFrameInput->setEnabled(true);
    endFrameInput->setEnabled(true);
    byPassInput->setEnabled(true);
}
void AutoFrameIterator::deactivate()
{
    active = false;
    startFrameInput->setEnabled(false);
    endFrameInput->setEnabled(false);
    byPassInput->setEnabled(false);

}
