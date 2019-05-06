#ifndef MAXMODEL_H
#define MAXMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>

#include <nodes/NodeDataModel>

#include <iostream>

class NodeGraphData;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class MaxModel : public NodeDataModel
{
    Q_OBJECT
public:
    MaxModel();
    virtual ~MaxModel() = default;

public:
    QString caption() const override {return QStringLiteral("Maximum");}
    bool captionVisible() const override {return true;}
    bool portCaptionVisible(PortType, PortIndex) const override {return true;}

    QString portCaption(PortType portType, PortIndex portIndex) const override
    {
        switch (portType)
        {
        case PortType::In:
            if (portIndex == 0)
            {
                return QStringLiteral("input");
            }
            break;
        case PortType::Out:
            return QStringLiteral("Result");
        default:
            break;
        }
        return QString();
    }

    QString name() const override {return QStringLiteral("Maximum");}

    QJsonObject save() const override;

public:

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex PortIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget * embeddedWidget() override;
    NodeValidationState validationState() const override;
    QString validationMessage() const override;

public slots:
    void onTextEdited();

private:

    void calculateResult();

    std::weak_ptr<NodeGraphData> _data;
    std::shared_ptr<NodeGraphData> _result;

    QLineEdit* _lineEdit;

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");
};

#endif // MINMODEL_H
