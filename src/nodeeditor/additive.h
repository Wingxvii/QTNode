#ifndef ADDITIVE_H
#define ADDITIVE_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>
#include <nodeeditor/nodegraphdata.h>

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class Additive : public NodeDataModel
{
public:
    Additive();
    virtual ~Additive() {}

public:
    QString caption() const override {return QStringLiteral("Additive");}
    bool captionVisible() const override {return true;}
    bool portCaptionVisible(PortType, PortIndex) const override {return true;}

    QString portCaption(PortType portType, PortIndex portIndex) const override
    {
        switch (portType)
        {
        case PortType::In:
            return QStringLiteral("input");
            break;
        case PortType::Out:
            return QStringLiteral("Result");
        default:
            break;
        }
        return QString();
    }

    QString name() const override {return QStringLiteral("Additive");}

    QJsonObject save() const override;

public:

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex PortIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget * embeddedWidget() override;
    NodeValidationState validationState() const override;
    QString validationMessage() const override;

private:
    void calculateResult();

    std::weak_ptr<NodeGraphData> _n1;
    std::weak_ptr<NodeGraphData> _n2;
    std::shared_ptr<NodeGraphData> _result;

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");
};

#endif // ADDITIVE_H
