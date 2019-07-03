#include "equalizehist.h"

EqualizeHist::EqualizeHist()
{
    //init
    layout = new QGridLayout;
    window = new QWidget;
    progressBar = new QLabel("Inactive");

    videoOut = std::make_shared<VideoGraphData>();

    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    //setlayout
    layout->addWidget(progressBar,1,1);
    window->setLayout(layout);

    buildContextWindow();
}

unsigned int EqualizeHist::nPorts(QtNodes::PortType PortType) const
{
    return 1;
}

QtNodes::NodeDataType EqualizeHist::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> EqualizeHist::outData(QtNodes::PortIndex port)
{
    return videoOut;
}

void EqualizeHist::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    videoIn = std::dynamic_pointer_cast<VideoGraphData>(data);
        if(videoIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            //instantiate output
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }

}

QtNodes::NodeValidationState EqualizeHist::validationState() const
{
    return modelValidationState;
}

QString EqualizeHist::validationMessage() const
{
    return modelValidationError;
}

QJsonObject EqualizeHist::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();

    return dataJson;
}

void EqualizeHist::restore(const QJsonObject & json)
{
    preCheck();
}

void EqualizeHist::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &EqualizeHist::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void EqualizeHist::preCheck()
{
    if(videoIn && videoIn->isReady && active){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void EqualizeHist::ShowContextMenu(const QPoint &pos)
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

void EqualizeHist::multiThreadedProcess()
{
    std::vector<cv::Mat> newVid;
    cv::Mat tempMat;

    for(cv::Mat frame : videoIn->_video){
        cv::equalizeHist(frame, tempMat);
        newVid.push_back(tempMat.clone());

    }
    videoOut->_video = newVid;

}

void EqualizeHist::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);

}

