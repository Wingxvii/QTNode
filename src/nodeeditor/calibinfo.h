#ifndef CALIBINFO_H
#define CALIBINFO_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>
#include <QDoubleValidator>

#include "calibdata.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class CalibInfo : public NodeDataModel{
Q_OBJECT
public:
    CalibInfo();
    virtual ~CalibInfo() {}

    QString caption() const override{
        return QStringLiteral("Calibration Info");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Calibration Info");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    QWidget* embeddedWidget() override {return window;}

public slots:

    void saveXData();
    void saveYData();
    void saveLengthData();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //data
    std::shared_ptr<CalibData> data;

private: //UI
    QWidget *window;
    QFormLayout* formLayout;
    QLineEdit* sizeX;
    QLineEdit* sizeY;
    QLineEdit* length;
    QLabel* sizeXText;
    QLabel* sizeYText;
    QLabel* lengthText;

    QRegExpValidator* intPos;
    QDoubleValidator* doublePos;


};

#endif // CALIBINFO_H
