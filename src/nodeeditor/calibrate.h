#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "DataTypes/pointsdata.h"
#include "DataTypes/imagedata.h"
#include "DataTypes/calibdata.h"
#include <QtConcurrent/QtConcurrent>

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QGridLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class Calibrate : public NodeDataModel{
Q_OBJECT
public:
    Calibrate();
    virtual ~Calibrate() {}

    QString caption() const override{
        return QStringLiteral("Calibrate Camera");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Calibrate Camera");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    //this is our work function
    void processData() override;
    //this is our check function
    void preCheck() override;

    //context menu slots
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



private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<PointsData> pointsIn;
    std::shared_ptr<CalibData> calibDataIn; //does not need ot be cleared
    std::shared_ptr<ImageData> cameraMat;
    std::shared_ptr<ImageData> distanceCoeff;

private: //UI
    QGridLayout *layout;



};

#endif // CALIBRATE_H
