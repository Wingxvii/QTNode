#ifndef GRAPHDISPLAYDATAMODEL_H
#define GRAPHDISPLAYDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
#include <QPushButton>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class GraphDisplayDataModel : public NodeDataModel
{
public:
    GraphDisplayDataModel();
    virtual ~GraphDisplayDataModel() {}

public:
    QString caption() const override {return QStringLiteral("Result Graph");}
    bool captionVisible() const override {return false;}
    QString name() const override {return QStringLiteral("Result Graph");}

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> data, int) override;

    QWidget *embeddedWidget() override {return _lineGraph;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    GraphStats stats;
    LineGraph* _lineGraph;

};

#endif // GRAPHDISPLAYDATAMODEL_H
