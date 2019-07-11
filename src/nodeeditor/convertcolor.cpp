#include "convertcolor.h"

convertColor::convertColor()
{
    //init
    layout = new QGridLayout;
    window =  new QWidget;
    codeSelection = new QListWidget;
    progressBar = new QLabel("Inactive");

    addCodes();

    //init out data
    videoOut = std::make_shared<VideoGraphData>();

    //input regulation
    //signed int
    intPos = new QRegExpValidator(QRegExp("-?\\d*"), this);


    //connections
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));
    connect(codeSelection, SIGNAL(currentRowChanged(int)), this, SLOT(preCheck()));

    //set layout
    layout->addWidget(codeSelection,1,1);
    layout->addWidget(progressBar,2,1);
    window->setLayout(layout);

    buildContextWindow();
}

unsigned int convertColor::nPorts(QtNodes::PortType PortType) const
{
    return 1;
}

QtNodes::NodeDataType convertColor::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<QtNodes::NodeData> convertColor::outData(QtNodes::PortIndex port)
{
    return videoOut;
}

void convertColor::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
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

QtNodes::NodeValidationState convertColor::validationState() const
{
    return modelValidationState;
}

QString convertColor::validationMessage() const
{
    return modelValidationError;
}

QJsonObject convertColor::save() const
{
    QJsonObject dataJson;
    dataJson["name"] = name();
    dataJson["codeIndex"] = codeSelection->currentIndex().row();


    return dataJson;
}

void convertColor::restore(const QJsonObject & json)
{
    if(json.contains("codeIndex")){
        codeSelection->setCurrentRow(json["codeIndex"].toInt());
    }


    preCheck();
}


void convertColor::processData()
{
    code = (cv::ColorConversionCodes)codeSelection->currentIndex().row();

    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &convertColor::multiThreadedProcess);
    functWatcher.setFuture(funct);

}

void convertColor::preCheck()
{

    if(videoIn && videoIn->isReady && active){
        processData();
    }
    else{
        if(videoOut){videoOut->unready();}
    }
}

void convertColor::ShowContextMenu(const QPoint &pos)
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

void convertColor::multiThreadedProcess()
{
    std::vector<cv::Mat> newVid;
    cv::Mat tempMat;

    //convert
    for(int x = 0; x < videoIn->_video.size(); x++){
        cv::Mat frame = videoIn->_video[x];
        cv::cvtColor(frame, tempMat, code);
        newVid.push_back(tempMat.clone());
        //LOG_JOHN() << x ;
    }

    //newVid
    videoOut->_video = newVid;
}

void convertColor::multiThreadedFinished()
{
    progressBar->setText("Finished");
    videoOut->ready();
    emit dataUpdated(0);

}

void convertColor::addCodes()
{
    codeSelection->addItem("COLOR_BGR2BGRA");
    codeSelection->addItem("COLOR_BGRA2BGR");
    codeSelection->addItem("COLOR_BGR2RGBA");
    codeSelection->addItem("COLOR_RGBA2BGR");
    codeSelection->addItem("COLOR_BGR2RGB");
    codeSelection->addItem("COLOR_BGRA2RGBA");
    codeSelection->addItem("COLOR_BGR2GRAY");
    codeSelection->addItem("COLOR_RGB2GRAY");
    codeSelection->addItem("COLOR_GRAY2BGR");
    codeSelection->addItem("COLOR_GRAY2BGRA");
    codeSelection->addItem("COLOR_BGRA2GRAY");
    codeSelection->addItem("COLOR_RGBA2GRAY");
    codeSelection->addItem("COLOR_BGR2BGR565");
    codeSelection->addItem("COLOR_RGB2BGR565");
    codeSelection->addItem("COLOR_BGR5652BGR");
    codeSelection->addItem("COLOR_BGR5652RGB");
    codeSelection->addItem("COLOR_BGRA2BGR565");
    codeSelection->addItem("COLOR_RGBA2BGR565");
    codeSelection->addItem("COLOR_BGR5652BGRA");
    codeSelection->addItem("COLOR_BGR5652RGBA");
    codeSelection->addItem("COLOR_GRAY2BGR565");
    codeSelection->addItem("COLOR_BGR5652GRAY");
    codeSelection->addItem("COLOR_BGR2BGR555");
    codeSelection->addItem("COLOR_RGB2BGR555");
    codeSelection->addItem("COLOR_BGR5552BGR");
    codeSelection->addItem("COLOR_BGR5552RGB");
    codeSelection->addItem("COLOR_BGRA2BGR555");
    codeSelection->addItem("COLOR_RGBA2BGR555");
    codeSelection->addItem("COLOR_BGR5552BGRA");
    codeSelection->addItem("COLOR_BGR5552RGBA");
    codeSelection->addItem("COLOR_GRAY2BGR555");
    codeSelection->addItem("COLOR_BGR5552GRAY");
    codeSelection->addItem("COLOR_BGR2XYZ");
    codeSelection->addItem("COLOR_RGB2XYZ");
    codeSelection->addItem("COLOR_XYZ2BGR");
    codeSelection->addItem("COLOR_XYZ2RGB");
    codeSelection->addItem("COLOR_BGR2YCrCb");
    codeSelection->addItem("COLOR_RGB2YCrCb");
    codeSelection->addItem("COLOR_YCrCb2BGR");
    codeSelection->addItem("COLOR_YCrCb2RGB");
    codeSelection->addItem("COLOR_BGR2HSV");
    codeSelection->addItem("COLOR_RGB2HSV");
    codeSelection->addItem("COLOR_BGR2Lab");
    codeSelection->addItem("COLOR_RGB2Lab");
    codeSelection->addItem("COLOR_BGR2Luv");
    codeSelection->addItem("COLOR_RGB2Luv");
    codeSelection->addItem("COLOR_BGR2HLS");
    codeSelection->addItem("COLOR_RGB2HLS");
    codeSelection->addItem("COLOR_HSV2BGR");
    codeSelection->addItem("COLOR_HSV2RGB");
    codeSelection->addItem("COLOR_Lab2BGR");
    codeSelection->addItem("COLOR_Lab2RGB");
    codeSelection->addItem("COLOR_Luv2BGR");
    codeSelection->addItem("COLOR_Luv2RGB");
    codeSelection->addItem("COLOR_HLS2BGR");
    codeSelection->addItem("COLOR_HLS2RGB");
    codeSelection->addItem("COLOR_BGR2HSV_FULL");
    codeSelection->addItem("COLOR_RGB2HSV_FULL");
    codeSelection->addItem("COLOR_BGR2HLS_FULL");
    codeSelection->addItem("COLOR_RGB2HLS_FULL");
    codeSelection->addItem("COLOR_HSV2BGR_FULL");
    codeSelection->addItem("COLOR_HSV2RGB_FULL");
    codeSelection->addItem("COLOR_HLS2BGR_FULL");
    codeSelection->addItem("COLOR_HLS2RGB_FULL");
    codeSelection->addItem("COLOR_LBGR2Lab");
    codeSelection->addItem("COLOR_LRGB2Lab");
    codeSelection->addItem("COLOR_LBGR2Luv");
    codeSelection->addItem("COLOR_LRGB2Luv");
    codeSelection->addItem("COLOR_Lab2LBGR");
    codeSelection->addItem("COLOR_Lab2LRGB");
    codeSelection->addItem("COLOR_Luv2LBGR");
    codeSelection->addItem("COLOR_Luv2LRGB");
    codeSelection->addItem("COLOR_BGR2YUV");
    codeSelection->addItem("COLOR_RGB2YUV");
    codeSelection->addItem("COLOR_YUV2BGR");
    codeSelection->addItem("COLOR_YUV2RGB");
    codeSelection->addItem("COLOR_YUV2RGB_NV12");
    codeSelection->addItem("COLOR_YUV2BGR_NV12");
    codeSelection->addItem("COLOR_YUV2RGB_NV21");
    codeSelection->addItem("COLOR_YUV2BGR_NV21");
    codeSelection->addItem("COLOR_YUV2RGBA_NV12");
    codeSelection->addItem("COLOR_YUV2BGRA_NV12");
    codeSelection->addItem("COLOR_YUV2RGBA_NV21");
    codeSelection->addItem("COLOR_YUV2BGRA_NV21");
    codeSelection->addItem("COLOR_YUV2RGB_YV12");
    codeSelection->addItem("COLOR_YUV2BGR_YV12");
    codeSelection->addItem("COLOR_YUV2RGB_IYUV");
    codeSelection->addItem("COLOR_YUV2BGR_IYUV");
    codeSelection->addItem("COLOR_YUV2RGBA_YV12");
    codeSelection->addItem("COLOR_YUV2BGRA_YV12");
    codeSelection->addItem("COLOR_YUV2RGBA_IYUV");
    codeSelection->addItem("COLOR_YUV2BGRA_IYUV");
    codeSelection->addItem("COLOR_YUV2GRAY_420");
    codeSelection->addItem("COLOR_YUV2RGB_UYVY");
    codeSelection->addItem("COLOR_YUV2BGR_UYVY");
    codeSelection->addItem("COLOR_YUV2RGBA_UYVY");
    codeSelection->addItem("COLOR_YUV2BGRA_UYVY");
    codeSelection->addItem("COLOR_YUV2RGB_YUY2");
    codeSelection->addItem("COLOR_YUV2BGR_YUY2");
    codeSelection->addItem("COLOR_YUV2RGB_YVYU");
    codeSelection->addItem("COLOR_YUV2BGR_YVYU");
    codeSelection->addItem("COLOR_YUV2RGBA_YUY2");
    codeSelection->addItem("COLOR_YUV2BGRA_YUY2");
    codeSelection->addItem("COLOR_YUV2RGBA_YVYU");
    codeSelection->addItem("COLOR_YUV2BGRA_YVYU");
    codeSelection->addItem("COLOR_YUV2GRAY_UYVY");
    codeSelection->addItem("COLOR_YUV2GRAY_YUY2");
    codeSelection->addItem("COLOR_RGBA2mRGBA");
    codeSelection->addItem("COLOR_mRGBA2RGBA");
    codeSelection->addItem("COLOR_RGB2YUV_I420");
    codeSelection->addItem("COLOR_BGR2YUV_I420");
    codeSelection->addItem("COLOR_RGBA2YUV_I420");
    codeSelection->addItem("COLOR_BGRA2YUV_I420");
    codeSelection->addItem("COLOR_RGB2YUV_YV12");
    codeSelection->addItem("COLOR_BGR2YUV_YV12");
    codeSelection->addItem("COLOR_RGBA2YUV_YV12");
    codeSelection->addItem("COLOR_BGRA2YUV_YV12");
    codeSelection->addItem("COLOR_BayerBG2BGR");
    codeSelection->addItem("COLOR_BayerGB2BGR");
    codeSelection->addItem("COLOR_BayerRG2BGR");
    codeSelection->addItem("COLOR_BayerGR2BGR");
    codeSelection->addItem("COLOR_BayerBG2GRAY");
    codeSelection->addItem("COLOR_BayerGB2GRAY");
    codeSelection->addItem("COLOR_BayerRG2GRAY");
    codeSelection->addItem("COLOR_BayerGR2GRAY");
    codeSelection->addItem("COLOR_BayerBG2BGR_VNG");
    codeSelection->addItem("COLOR_BayerGB2BGR_VNG");
    codeSelection->addItem("COLOR_BayerRG2BGR_VNG");
    codeSelection->addItem("COLOR_BayerGR2BGR_VNG");
    codeSelection->addItem("COLOR_BayerBG2BGR_EA");
    codeSelection->addItem("COLOR_BayerGB2BGR_EA");
    codeSelection->addItem("COLOR_BayerRG2BGR_EA");
    codeSelection->addItem("COLOR_BayerGR2BGR_EA");
    codeSelection->addItem("COLOR_BayerBG2BGRA");
    codeSelection->addItem("COLOR_BayerGB2BGRA");
    codeSelection->addItem("COLOR_BayerRG2BGRA");
    codeSelection->addItem("COLOR_BayerGR2BGRA");
    codeSelection->addItem("COLOR_COLORCVT_MAX");
}


