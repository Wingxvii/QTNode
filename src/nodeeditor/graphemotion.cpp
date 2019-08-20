#include "graphemotion.h"



GraphEmotion::GraphEmotion()
{
    layout = new QGridLayout;
    window = new QWidget;
    indexesWidget = new QWidget;

    TotalFrames = new QLabel("Total number of frames: ");
    xRangeControl = new QSlider(Qt::Horizontal);
    yRangeControl = new QSlider(Qt::Vertical);
    xPositionControl = new QScrollBar(Qt::Horizontal);
    yPositionControl = new QScrollBar(Qt::Vertical);
    yPositionControl->setInvertedAppearance(true);
    xRangeControl->setMinimum(1);
    yRangeControl->setMinimum(1);

    displayPlot = new QwtPlot();
    displayPlot->setAutoReplot(false);

    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,2000);
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom, 0,1000);
    displayPlot->setTitle("Emotions Detected");
    displayPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Emotion Distribution Values");
    displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Frame Number");

    connect(xRangeControl, SIGNAL(valueChanged(int)), this, SLOT(updateX()));
    connect(yRangeControl, SIGNAL(valueChanged(int)), this, SLOT(updateY()));
    connect(xPositionControl, SIGNAL(valueChanged(int)), this, SLOT(updateX()));
    connect(yPositionControl, SIGNAL(valueChanged(int)), this, SLOT(updateY()));

    layout->addWidget(displayPlot,1,3,7,1);
    layout->addWidget(TotalFrames,1,4);
    layout->addWidget(indexesWidget,2,4,6,1);
    layout->addWidget(xRangeControl,9,3);
    layout->addWidget(yRangeControl,1,1,7,1);
    layout->addWidget(xPositionControl,8,3);
    layout->addWidget(yPositionControl,1,2,7,1);

    curves = new std::map<std::string, QwtPlotCurve*>();

    window->setLayout(layout);
    buildContextWindow();

}

unsigned int GraphEmotion::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType GraphEmotion::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return EmotionData().type();
}

std::shared_ptr<QtNodes::NodeData> GraphEmotion::outData(QtNodes::PortIndex port)
{
    return dataIn;
}

void GraphEmotion::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{

    switch(location){
    case 0:
    dataIn = std::dynamic_pointer_cast<EmotionData>(data);
        if(dataIn){
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


QtNodes::NodeValidationState GraphEmotion::validationState() const
{
    return modelValidationState;
}

QString GraphEmotion::validationMessage() const
{
    return modelValidationError;
}

void GraphEmotion::processData()
{
    displayPlot->detachItems();

    //setup var data
    totalFrames = dataIn->_valuePercentages.size();
    LOG_JOHN() <<"Starting";
    if(!indexes->empty()){
        LOG_JOHN() << "Try to clear";
        indexes->clear();
    }

    for(std::string label : dataIn->_labels){
        //curve setup
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setBrush(Qt::BrushStyle::NoBrush);
        curve->setStyle(QwtPlotCurve::CurveStyle::Lines);
        QColor color = (Qt::GlobalColor)(7 + curves->size());
        color.setAlpha(200);
        curve->setPen(color);

        int maxValue = 0;

        LOG_JOHN() <<"Set Up a Curve";

        //fill curve with data
        QVector<QPointF> samples;
        for(int counter = 0; counter < dataIn->_valuePercentages.size(); counter++){
            int num = dataIn->_valuePercentages[counter][curves->size()];
            QPoint plot = QPoint(counter, num);
            samples.push_front(plot);
            if(maxValue < num){maxValue = num;}
            LOG_JOHN() <<"Added Sample";
        }
        curve->setSamples(samples);
        curve->attach(displayPlot);
        curve->setVisible(true);

        //setup ranges
        displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,maxValue);
        displayPlot->setAxisScale(QwtPlot::Axis::xBottom, 0,totalFrames);

        xRangeControl->setMaximum(totalFrames);
        yRangeControl->setMaximum(maxValue);
        xPositionControl->setMaximum(totalFrames);
        yPositionControl->setMaximum(maxValue);

        //build label
        QLabel *index = new QLabel();

        QPalette palette = index->palette();
        palette.setColor(index->foregroundRole(), color);
        index->setPalette(palette);
        index->setText(QString::fromStdString(label));

        indexes->push_back(index);

        //build pair
        std::pair<std::string, QwtPlotCurve*> labelData;
        labelData.first = label;
        labelData.second = curve;

        //send pair into vector
        curves->emplace(labelData);
    }

    LOG_JOHN() <<"Done";

    updateUI();
    displayPlot->replot();

}

void GraphEmotion::preCheck()
{
    if(dataIn && dataIn->isReady && active){
        processData();
    }

}

void GraphEmotion::updateUI()
{
    //setup indexes
    indexesLayout = new QGridLayout;

    for(QLabel* label : *indexes){
        indexesLayout->addWidget(label);
    }
    indexesWidget->setLayout(indexesLayout);

    //setup total frame number
    TotalFrames->setText("Total number of frames: " + QString::number(totalFrames));
}


void GraphEmotion::ShowContextMenu(const QPoint &pos)
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

void GraphEmotion::updateX()
{
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom, xPositionControl->value() - xRangeControl->value(),xPositionControl->value() + xRangeControl->value());
    displayPlot->replot();
    LOG_JOHN() <<"Replotted X";

}

void GraphEmotion::updateY()
{
    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, yPositionControl->value() - yRangeControl->value(),yPositionControl->value() + yRangeControl->value());
    displayPlot->replot();
    LOG_JOHN() <<"Replotted Y";

}

