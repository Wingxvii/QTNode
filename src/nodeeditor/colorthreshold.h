#ifndef COLORTHRESHOLD_H
#define COLORTHRESHOLD_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

#include "DataTypes/videographdata.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QSlider>
#include <QAbstractSlider>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class ColorThreshold : public NodeDataModel{
Q_OBJECT

public:
    ColorThreshold();
    virtual ~ColorThreshold(){}

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

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void saveHData();
    void saveSData();
    void saveVData();

    void updateHData();
    void updateSData();
    void updateVData();


private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

public slots:
    //iterates throught the video, saving frames to a image vector based on parameters
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

public: //multithread

    void multiThreadedProcess();

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressBar;
public slots:
    void multiThreadedFinished();

private: //Data Ports
    std::shared_ptr<VideoGraphData> inVideo;
    std::shared_ptr<VideoGraphData> outVideo;

private: //UI
    QGridLayout* formLayout;

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

    QLabel* HMinValue;
    QLabel* HMaxValue;
    QLabel* SMinValue;
    QLabel* SMaxValue;
    QLabel* VMinValue;
    QLabel* VMaxValue;


private: //locals

    int HMin = 0;
    int HMax = 255;
    int SMin = 0;
    int SMax = 255;
    int VMin = 0;
    int VMax = 255;


};

#endif // COLORTHRESHOLD_H
