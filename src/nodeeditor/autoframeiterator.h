#ifndef AUTOFRAMEITERATOR_H
#define AUTOFRAMEITERATOR_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <iostream>

//data types
#include "imagevector.h"
#include "videographdata.h"

//QT widgets
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QProgressBar>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class AutoFrameIterator : public NodeDataModel{
    Q_OBJECT
public:
    AutoFrameIterator();
    virtual ~AutoFrameIterator(){}

    QString caption() const override{
        return QStringLiteral("Automatic Frame Iterator");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Automatic Frame Iterator");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget* embeddedWidget() override {return window;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");    \



public slots: //slots
    //iterates throught the video, saving frames to a image vector based on parameters
    void startIteration();
    //calculates the values displayed. This is called whenever a value or input is updated
    void calcValues();

private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<ImageVector> imagesOut;

private: //locals
    int totalFrames = 0;
    int projectedSamples = 0;
    int startFrame = 0;
    int endFrame = -1;
    int byPass = 0;
    int maxSamples = 0;

private: //UI
    QWidget *window;
    QGridLayout *layout;

    QLabel *totalFramesDisplay;
    QLabel *totalFramesLabel;
    QLabel *startLabel;
    QLineEdit* startFrameInput;
    QLabel *endLabel;
    QLineEdit* endFrameInput;
    QLabel *byPassLabel;
    QLineEdit *byPassInput;
    QLabel *maxSampleLabel;
    QLineEdit *maxSamplesInput;
    QProgressBar *progressBar;
    QLabel *projectedSamplesLabel;
    QLabel *projectedSamplesDisplay;
    QPushButton *startButton;

    QRegExpValidator* intPos;

};
#endif // AUTOFRAMEITERATOR_H
