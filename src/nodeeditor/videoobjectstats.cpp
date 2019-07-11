#include "videoobjectstats.h"
#include <qwt_symbol.h>

double GetMedian(QVector<double> daArray) {
    int iSize = daArray.size();

    // Allocate an array of the same size and sort it.
    double* dpSorted = new double[iSize];
    for (int i = 0; i < iSize; ++i) {
        dpSorted[i] = daArray[i];
    }
    for (int i = iSize - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (dpSorted[j] > dpSorted[j+1]) {
                double dTemp = dpSorted[j];
                dpSorted[j] = dpSorted[j+1];
                dpSorted[j+1] = dTemp;
            }
        }
    }

    // Middle or average of middle values in the sorted array.
    double dMedian = 0.0;
    if ((iSize % 2) == 0) {
        dMedian = (dpSorted[iSize/2] + dpSorted[(iSize/2) - 1])/2.0;
    } else {
        dMedian = dpSorted[iSize/2];
    }
    delete [] dpSorted;
    return dMedian;
}

double GetMode(QVector<double> daArray) {
    int iSize = daArray.size();


    // Allocate an int array of the same size to hold the
    // repetition count
    int* ipRepetition = new int[iSize];
    for (int i = 0; i < iSize; ++i) {
        ipRepetition[i] = 0;
        int j = 0;
        bool bFound = false;
        while ((j < i) && (daArray[i] != daArray[j])) {
            if (daArray[i] != daArray[j]) {
                ++j;
            }
        }
        ++(ipRepetition[j]);
    }
    int iMaxRepeat = 0;
    for (int i = 1; i < iSize; ++i) {
        if (ipRepetition[i] > ipRepetition[iMaxRepeat]) {
            iMaxRepeat = i;
        }
    }
    delete [] ipRepetition;
    return daArray[iMaxRepeat];
}

double GetMean(QVector<double> daArray) {
    int iSize = daArray.size();

    double dSum = daArray[0];
    for (int i = 1; i < iSize; ++i) {
        dSum += daArray[i];
    }
    return dSum/iSize;
}


VideoObjectStats::VideoObjectStats()
{
    layout = new QGridLayout;
    window = new QWidget;
    MaxObjs = new QLabel("Maximum of objects found:");
    MeanObjs = new QLabel("Mean number of objects found:");
    MeanObjsStripped = new QLabel("Mean number of objects found with stripped frames:");
    MedianObjs = new QLabel("Median number of objects found:");
    MedianStripped = new QLabel("Median number of objects found with stripped frames:");
    ModeObjsStripped = new QLabel("Mode of objects of stripped frames:");
    TotalFrames = new QLabel("Total number of frames: ");


    displayPlot = new QwtPlot();
    displayPlot->setAutoReplot(false);

    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,5);
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom, 0,1000);
    displayPlot->setTitle("Object Detection Rates");
    displayPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Number of objects Detected");
    displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Frame Number");

    //connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(displayPlot,1,1,7,1);
    layout->addWidget(MaxObjs,1,2);
    layout->addWidget(MeanObjs,2,2);
    layout->addWidget(MeanObjsStripped,3,2);
    layout->addWidget(MedianObjs,4,2);
    layout->addWidget(MedianStripped,5,2);
    layout->addWidget(ModeObjsStripped,6,2);
    layout->addWidget(TotalFrames,7,2);

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int VideoObjectStats::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType VideoObjectStats::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> VideoObjectStats::outData(QtNodes::PortIndex port)
{
    return inBoxes;
}

void VideoObjectStats::setInData(std::shared_ptr<QtNodes::NodeData>data , int location)
{

    switch(location){
    case 0:
    inBoxes = std::dynamic_pointer_cast<DetectionBoxesData>(data);
        if(inBoxes){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();

            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState VideoObjectStats::validationState() const
{
    return modelValidationState;
}

QString VideoObjectStats::validationMessage() const
{
    return modelValidationError;
}

QJsonObject VideoObjectStats::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;
}

void VideoObjectStats::restore(const QJsonObject &)
{

}

void VideoObjectStats::processData()
{
    displayPlot->detachItems();
    curve = new QwtPlotCurve();
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    QVector<QPointF> samples;
    QVector<double> sampleValues;
    QVector<double> strippedValues;

    QColor c = Qt::red;
    c.setAlpha(200);

    curve->setPen(c);
    curve->setBrush(c);

    //setup vars
    totalFrames = inBoxes->_boxes.size();
    maxObjs = 0;

    for(int counter = 0; counter < inBoxes->_boxes.size() ; counter++){
        int num = inBoxes->_boxes[counter].size();
        QPoint plot = QPoint(counter,num);
        samples.push_back(plot);
        sampleValues.push_back(plot.y());
        if(num > 0){strippedValues.push_back(plot.y());}
        LOG_JOHN() <<"Added Sample";

        if(maxObjs < num){maxObjs = num;}
    }

    meanObjs = GetMean(sampleValues);
    meanStripped = GetMean(strippedValues);
    medianObjs = GetMedian(sampleValues);
    medianStripped = GetMedian(strippedValues);
    modestripped = GetMode(strippedValues);

    curve->setSamples(samples);
    curve->attach(displayPlot);
    curve->setVisible(true);

    LOG_JOHN() <<"Drew Graph";

    //recalc axis scale
    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,maxObjs);
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom, 0,totalFrames);

    updateUI();

    displayPlot->replot();
}

void VideoObjectStats::preCheck()
{
    if(inBoxes && inBoxes->isReady && active){
        processData();
    }
}

void VideoObjectStats::updateUI()
{
    MaxObjs->setText("Maximum of objects found: " + QString::number(maxObjs));
    MeanObjs->setText("Mean number of objects found: "+ QString::number(meanObjs));
    MeanObjsStripped->setText("Mean number of objects found with stripped frames: "+ QString::number(meanStripped));
    MedianObjs->setText("Median number of objects found: "+ QString::number(medianObjs));
    MedianStripped->setText("Median number of objects found with stripped frames: "+ QString::number(medianStripped));
    ModeObjsStripped->setText("Mode of objects of stripped frames: "+ QString::number(modestripped));
    TotalFrames->setText("Total number of frames: " + QString::number(totalFrames));
}

void VideoObjectStats::ShowContextMenu(const QPoint &pos)
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

void VideoObjectStats::multiThreadedProcess()
{

}

void VideoObjectStats::multiThreadedFinished()
{

}





