#ifndef GRAPHEMOTION_H
#define GRAPHEMOTION_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/emotiondata.h"

//QT widgets
#include <QLabel>
#include <QGridLayout>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
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

class GraphEmotion : public NodeDataModel
{Q_OBJECT
public:
    GraphEmotion();
    virtual ~GraphEmotion(){}

public: //required classes
    QString caption() const override{
        return QStringLiteral("Emotion Stats");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Emotion Stats");
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

    //QJsonObject save() const override;
    //virtual void restore(QJsonObject const &) override;

public slots: //required slots
    void processData() override;
    void preCheck() override;
    void updateUI();

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}
public: //multithread


public slots:

    void updateX();
    void updateY();

    void boxChecked(int state);
    void initLabels();
private: //ports
    std::shared_ptr<EmotionData> dataIn;

private: //ui
    QGridLayout *layout;
    QwtPlot *displayPlot;
    QLabel *TotalFrames;

    std::vector<QLabel*> *indexes = new std::vector<QLabel*>();
    std::vector<QCheckBox*> *indexBoxes = new std::vector<QCheckBox*>();

    QWidget *indexesWidget;
    QGridLayout *indexesLayout;

    QSlider *xRangeControl;
    QSlider *yRangeControl;
    QScrollBar *xPositionControl;
    QScrollBar *yPositionControl;

private: //locals
    int totalFrames;
    bool point;
    std::map<std::string, QwtPlotCurve*> *curves;
    std::vector<bool> visible;
};

#endif // GRAPHEMOTION_H
