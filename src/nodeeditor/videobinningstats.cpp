#include "videobinningstats.h"
#include <qwt_column_symbol.h>


class DistroChartItem: public QwtPlotBarChart
{
public:
    DistroChartItem():
        QwtPlotBarChart( "Page Hits" )
    {
        setLegendMode( QwtPlotBarChart::LegendBarTitles );
        setLegendIconSize( QSize( 10, 14 ) );
    }

    void addDistro( const QString &distro, const QColor &color )
    {
        d_colors += color;
        d_distros += distro;
        itemChanged();
    }

    virtual QwtColumnSymbol *specialSymbol(
        int index, const QPointF& ) const
    {
        // we want to have individual colors for each bar

        QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol->setLineWidth( 2 );
        symbol->setFrameStyle( QwtColumnSymbol::Raised );

        QColor c( Qt::white );
        if ( index >= 0 && index < d_colors.size() )
            c = d_colors[ index ];

        symbol->setPalette( c );

        return symbol;
    }

    virtual QwtText barTitle( int sampleIndex ) const
    {
        QwtText title;
        if ( sampleIndex >= 0 && sampleIndex < d_distros.size() )
            title = d_distros[ sampleIndex ];

        return title;
    }

private:
    QList<QColor> d_colors;
    QList<QString> d_distros;
};


VideoBinningStats::VideoBinningStats()
{
    layout = new QGridLayout;
    window = new QWidget;
    binInput = new QLineEdit();
    binSelect = new QComboBox();
    binSelect->addItem("Graph by frames per bin");
    binSelect->addItem("Graph by number of bins");
    binSelect->addItem("Graph by unique occurances");
    binSelect->setCurrentIndex(1);

    displayPlot = new QwtPlot();
    displayPlot->setAutoReplot(false);
    displayPlot->setFrameStyle( QFrame::Box | QFrame::Sunken );

    displayPlot->setTitle("Binning Stats");
    displayPlot->setAxisScale(QwtPlot::Axis::yLeft, 0,5);
    displayPlot->setAxisTitle(QwtPlot::Axis::yLeft, "Hits");
    displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Bins");

    //input regulation unsigned int
    QRegExpValidator* intPos = new QRegExpValidator(QRegExp("\\d*"), this);
    binInput->setValidator(intPos);

    connect(binSelect, SIGNAL(activated(int)), this, SLOT(changeBarType()));
    connect(binInput, SIGNAL(returnPressed()), this, SLOT(preCheck()));

    layout->addWidget(displayPlot,2,1,1,2);
    layout->addWidget(binSelect,1,1);
    layout->addWidget(binInput,1,2);
    window->setLayout(layout);
    buildContextWindow();
}

unsigned int VideoBinningStats::nPorts(QtNodes::PortType portType) const
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

QtNodes::NodeDataType VideoBinningStats::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    return DetectionBoxesData().type();
}

std::shared_ptr<QtNodes::NodeData> VideoBinningStats::outData(QtNodes::PortIndex port)
{
    return inBoxes;
}

void VideoBinningStats::setInData(std::shared_ptr<QtNodes::NodeData>data , int location)
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

QtNodes::NodeValidationState VideoBinningStats::validationState() const
{
    return modelValidationState;
}

QString VideoBinningStats::validationMessage() const
{
    return modelValidationError;
}

QJsonObject VideoBinningStats::save() const
{
    QJsonObject dataJson;

    dataJson["name"] = name();
    return dataJson;
}

void VideoBinningStats::restore(const QJsonObject &)
{

}

void VideoBinningStats::processData()
{

    displayPlot->detachItems();
    d_barChartItem = new DistroChartItem();
    d_barChartItem->setRenderHint(QwtPlotItem::RenderAntialiased);

    QPalette canvasPalette( QColor( "Plum" ) );
    canvasPalette.setColor( QPalette::Foreground, QColor( "Indigo" ) );
    displayPlot->setPalette( canvasPalette );

    QVector< double > samples = QVector<double>();
    QVector<QString> sampleNames;

    int maxHits = 0;

    if(barType != 2){
        for(int counter = 0; counter < bins; counter++){
            int temp = 0;
            for(int counter2 = 0; counter2 < framesPerBin; counter2++){
                if(counter*framesPerBin + counter2 < inBoxes->_boxes.size()){
                    temp += inBoxes->_boxes[counter*framesPerBin + counter2].size();
                }
            }
            samples += temp;
            if(temp > maxHits){maxHits = temp;}
            d_barChartItem->addDistro(QString::number(counter*framesPerBin) + "-" + QString::number(((counter+1)*framesPerBin)-1), QColor("SteelBlue"));
        }

    }else{
        bins = 0;
        //fill a vector with hits
        for(std::vector<cv::Rect> boxes : inBoxes->_boxes){
            while(samples.size() < boxes.size()+1){
                samples += 0.0;
            }
            LOG_JOHN() <<boxes.size() << "/" << samples.size();
            samples[boxes.size()]+=1.0;
        }
        LOG_JOHN() <<"twoa";

        //send vector to bar
        for(double unique : samples){
            d_barChartItem->addDistro(QString::number(bins) + " Hits", QColor("SteelBlue"));
            if(unique > maxHits){maxHits = unique;}
            bins++;
        }
    }

    LOG_JOHN() <<maxHits << "Max Hits";

    displayPlot->setAxisScale(QwtPlot::Axis::yLeft,0, maxHits);
    displayPlot->setAxisScale(QwtPlot::Axis::xBottom,0, bins);

    d_barChartItem->setSamples(samples);
    d_barChartItem->attach(displayPlot);
    d_barChartItem->setVisible(true);

    LOG_JOHN() <<"Drew Graph";

    displayPlot->replot();
}

void VideoBinningStats::preCheck()
{
    if(barType == 0 && !binInput->text().isEmpty()){
        framesPerBin = binInput->text().toInt();
        updateUI();
    }else if(barType == 1 && !binInput->text().isEmpty()){
        bins = binInput->text().toInt();
        updateUI();
    }else if(barType == 2){
        updateUI();
    }
    else{
        isReady = false;
    }

    if(inBoxes && inBoxes->isReady && active && isReady){
        processData();
    }

}

void VideoBinningStats::updateUI()
{
    totalFrames = inBoxes->_boxes.size();

    if(barType == 0){
        if(framesPerBin < totalFrames){
            bins = 1;
            for(int counter = 0; counter < totalFrames; counter += framesPerBin){
                ++bins;
            }
            isReady = true;
        }else{
            isReady = false;
        }

    }else if(barType == 1){
        if(bins < totalFrames){
            if(bins == 1){
                framesPerBin = totalFrames;
            }else{
                framesPerBin = (int)(totalFrames/(bins-1));
            }
            isReady = true;
        }else{
            isReady = false;
        }

    }else{
        isReady = true;
    }
}

void VideoBinningStats::changeBarType()
{
    switch(binSelect->currentIndex()){
        case 0:
            displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Bins");
            barType = 0;
        break;
    case 1:
        displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Bins");
        barType = 1;
        break;
    case 2:
        displayPlot->setAxisTitle(QwtPlot::Axis::xBottom, "Occurances");
        barType = 2;
        break;
    }
    preCheck();
}

void VideoBinningStats::ShowContextMenu(const QPoint &pos)
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
