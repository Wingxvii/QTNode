#ifndef DEBUGGETCORNERS_H
#define DEBUGGETCORNERS_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "analyzer/linkmanager.h"

#include <iostream>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class DebugGetCorners : public NodeDataModel{
    Q_OBJECT


public:
    DebugGetCorners();
    virtual ~DebugGetCorners() {}

    QString caption() const override{
        return QStringLiteral("(DEBUG) Find Corners");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("(DEBUG) Find Corners");
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

public slots:
    void processData() override;
    void preCheck() override;
    void updateUI();

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<ImageData> imageIn;
    std::shared_ptr<ImageData> imageOut;
    std::shared_ptr<PointData> cornersOut;
    std::shared_ptr<CalibData> dataIn;

private: //ui

    QGridLayout* layout;
    QLabel* cornersFoundLabel;
    bool cornersFound;

};

#endif // DebugGetCorners_H
