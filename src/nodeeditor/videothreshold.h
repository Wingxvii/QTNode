#ifndef VIDEOTHRESHOLD_H
#define VIDEOTHRESHOLD_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "videographdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QSlider>
#include <QAbstractSlider>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class VideoThreshold : public NodeDataModel{
Q_OBJECT

public:
    VideoThreshold();
    virtual ~VideoThreshold(){}

public:

    QString caption() const override{
        return QStringLiteral("Video Threshold");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Video Threshold");
    }

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget* embeddedWidget() override {return window;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void saveHData();
    void saveSData();
    void saveVData();
    void applyThreshold();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //Data Ports
    std::shared_ptr<VideoGraphData> inVideo;
    std::shared_ptr<VideoGraphData> outVideo;

private: //UI
    QWidget *window;
    QFormLayout* formLayout;
    QSlider* HMinEdit;
    QSlider* HMaxEdit;
    QSlider* SMinEdit;
    QSlider* SMaxEdit;
    QSlider* VMinEdit;
    QSlider* VMaxEdit;
    QLabel* HMinText;
    QLabel* HMaxText;
    QLabel* SMinText;
    QLabel* SMaxText;
    QLabel* VMinText;
    QLabel* VMaxText;
    QPushButton* button;

    int HMin = 0;
    int HMax = 255;
    int SMin = 0;
    int SMax = 255;
    int VMin = 0;
    int VMax = 255;


};


#endif // VIDEOTHRESHOLD_H
