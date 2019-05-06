#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "pointsdata.h"
#include "imagedata.h"
#include "calibdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

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

    QWidget* embeddedWidget() override {return button;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void startCalibration();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<PointsData> pointsIn;
    std::shared_ptr<CalibData> calibDataIn;
    std::shared_ptr<ImageData> cameraMat;
    std::shared_ptr<ImageData> distanceCoeff;
    QPushButton* button;


};

#endif // CALIBRATE_H
