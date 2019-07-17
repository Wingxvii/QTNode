#ifndef VIDEOBINNINGSTATS_H
#define VIDEOBINNINGSTATS_H


#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/detectionboxesdata.h"

//QT widgets
#include <QLabel>
#include <QGridLayout>
#include <qwt_plot.h>
#include <qwt_plot_barchart.h>
#include <QCheckBox>
#include <QSlider>
#include <QScrollBar>
#include <QLineEdit>
#include <QComboBox>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class DistroChartItem;

class VideoBinningStats: public NodeDataModel
{   Q_OBJECT

public:
    VideoBinningStats();
    virtual ~VideoBinningStats(){};

public: //required classes
    QString caption() const override{
        return QStringLiteral("Binning Stats");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Binning Stats");
    }
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

public slots: //required slots
    void processData() override;
    void preCheck() override;
    void updateUI();
    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}
public: //multithread

    //void multiThreadedProcess();

    //QFuture<void> funct;
    //QFutureWatcher<void> functWatcher;
   // QLabel *progressBar;

public slots:
    //void multiThreadedFinished();
    void changeBarType();

private: //ports
    std::shared_ptr<DetectionBoxesData> inBoxes;

private: //ui
    QGridLayout *layout;
    QwtPlot *displayPlot;

    QComboBox *binSelect;
    QLineEdit *binInput;

    QRegExpValidator* intPos;



private: //locals
    int framesPerBin;
    int bins;
    int maxObjs;
    int totalFrames;
    int barType;
    //bool point;
    QwtPlotBarChart *bar;
    bool isReady = false;
    DistroChartItem *d_barChartItem;

};

#endif // VIDEOBINNINGSTATS_H
