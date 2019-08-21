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
            if(dataIn->isReady && active){
                initLabels();
            }
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
    int maxValue = 0;

    for(int counter = 0; counter < indexBoxes->size(); counter++){
        std::string label = dataIn->_labels[counter];

        if(!visible[counter]){
            continue;
        }
        //curve setup
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setBrush(Qt::BrushStyle::NoBrush);
        curve->setStyle(QwtPlotCurve::CurveStyle::Lines);
        QColor color = (Qt::GlobalColor)(7 + counter);
        color.setAlpha(200);
        curve->setPen(color);


        LOG_JOHN() <<"Set Up a Curve";

        //fill curve with data
        QVector<QPointF> samples;
        for(int counter2 = 0; counter2 < dataIn->_valuePercentages.size(); counter2++){
            int num = dataIn->_valuePercentages[counter2][counter];
            QPoint plot = QPoint(counter2, num);
            samples.push_front(plot);
            if(maxValue < num){maxValue = num;}
            LOG_JOHN() <<"Added Sample";
        }
        curve->setSamples(samples);
        curve->attach(displayPlot);
        curve->setVisible(true);

        //build pair
        std::pair<std::string, QwtPlotCurve*> labelData;
        labelData.first = label;
        labelData.second = curve;

        //send pair into vector
        curves->emplace(labelData);
    }

    //setup ranges
    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,maxValue);
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom, 0,totalFrames);

    xRangeControl->setMaximum(totalFrames);
    yRangeControl->setMaximum(maxValue);
    xPositionControl->setMaximum(totalFrames);
    yPositionControl->setMaximum(maxValue);

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

    for(int counter = 1; counter <= indexes->size(); counter++){
        indexesLayout->addWidget(indexes->at(counter-1), counter, 1);
        indexesLayout->addWidget(indexBoxes->at(counter-1), counter, 2);

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

void GraphEmotion::boxChecked(int state)
{
    for(int counter = 0; counter < indexBoxes->size(); counter++){
        if(indexBoxes->at(counter)->isChecked()){
            LOG_JOHN() << "Checked";
            visible[counter] = true;
        }else{
            visible[counter]= false;
        }
    }
    preCheck();
}

void GraphEmotion::initLabels()
{
    if(!indexes->empty()){
        LOG_JOHN() << "Try to clear";
        indexes->clear();
    }
    if(!indexBoxes->empty()){
        LOG_JOHN() << "Try to clear";
        indexBoxes->clear();
    }
    if(!visible.empty()){
        LOG_JOHN() << "Try to clear";
        visible.clear();
    }


    //for(std::string label : dataIn->_labels){
    for(int counter = 0; counter < dataIn->_labels.size(); counter++){

        QColor color = (Qt::GlobalColor)(7 + counter);
        std::string label = dataIn->_labels[counter];

        //build label
        QLabel *index = new QLabel();
        QCheckBox *button = new QCheckBox();

        visible.push_back(true);
        QPalette palette = index->palette();
        palette.setColor(index->foregroundRole(), color);
        index->setPalette(palette);
        index->setText(QString::fromStdString(label));

        button->setChecked(true);

        connect(button, SIGNAL(stateChanged(int)), this, SLOT(boxChecked(int)));

        indexBoxes->push_back(button);

        indexes->push_back(index);
    }
}

