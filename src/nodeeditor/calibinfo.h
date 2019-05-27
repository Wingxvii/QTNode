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

#include "DataTypes/calibdata.h"

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

public slots:

    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

private: //data
    std::shared_ptr<CalibData> data;

private: //UI
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
