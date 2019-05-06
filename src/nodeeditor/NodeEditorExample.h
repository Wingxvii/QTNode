#ifndef NODEEDITOREXAMPLE
#define NODEEDITOREXAMPLE

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>
#include <nodeeditor/NodeEditorDataExample.h>

#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class NodeEditorExample : public NodeDataModel
{
public:
    NodeEditorExample()
    {
        label = new QLabel("Example Label");
    }
    virtual ~NodeEditorExample() {}

public:

    //This is the title for the node
    QString caption() const override {return QStringLiteral("Example");}
    //This is whether we want the caption to be visible or not
    bool captionVisible() const override {return true;}

    //Whether we want port captions to be visible
    bool portCaptionVisible(PortType, PortIndex) const override {return true;}
    //This is supposed to change the names on the port captions but i couldnt get it to work
    QString portCaption(PortType portType, PortIndex portIndex) const override { }


    QString name() const override {return QStringLiteral("Additive");}

    //If you define this it gets called when you save the node editor
    QJsonObject save() const override {}

public:

    //How many ports you want for input and output
    unsigned int nPorts(PortType portType) const override
    {
        unsigned int result = 1;

        switch (portType)
        {
        case PortType::In:
            result = 2;
            break;

        case PortType::Out:
            result = 1;

        default:
            break;
        }

        return result;
    }

    //What type of data you want for each type of port and port index if there are multiple ports
    NodeDataType dataType(PortType portType, PortIndex PortIndex) const override
    {
        return NodeEditorDataExample.type();
    }


    //This is what transfer the data from this node to the next
    //The port index can be used determine what output index is being used
    std::shared_ptr<NodeData> outData(PortIndex) override
    {
        return std::make_shared<NodeEditorDataExample>(5);
    }

    //This is where data is piped into this node, the shared ptr is the NodeData and the int is the index of the input port
    void setInData(std::shared_ptr<NodeData>, int) override
    {

    }

    //This is what widget is displayed for the node
    QWidget * embeddedWidget() override { return label; }

    //This is what validation statte the node is in
    NodeValidationState validationState() const override
    {
        return modelValidationError;
    }

    //this is what validation message to show
    QString validationMessage() const override
    {
        return modelValidationState;
    }

private:
    QLabel * label;
    std::shared_ptr<NodeGraphData> _result;

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");
};

#endif // ADDITIVE_H
