#ifndef BACKGROUNDSUBTRACTION_H
#define BACKGROUNDSUBTRACTION_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
#include <QPushButton>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>
#include "videographdata.h"

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class BackgroundSubtraction : public NodeDataModel
{
    Q_OBJECT
public:
    BackgroundSubtraction();
    virtual ~BackgroundSubtraction() {}

public:
    QString caption() const override {return QStringLiteral("Background Subtraction");}
    bool captionVisible() const override {return true;}
    bool portCaptionVisible(PortType, PortIndex) {return true;}

    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QString name() const override {return QStringLiteral("Background Subtraction");}

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget *embeddedWidget() override {return widget;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QLabel* widget;

    std::shared_ptr<VideoGraphData> inData;
    std::shared_ptr<VideoGraphData> threshold;
    std::shared_ptr<VideoGraphData> output;

};

#endif // BACKGROUNDSUBTRACTION_H

