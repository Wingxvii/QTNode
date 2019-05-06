#ifndef MINMODEL_H
#define MINMODEL_H

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

class MinModel : public NodeDataModel
{
    Q_OBJECT
public:
    MinModel();
    virtual ~MinModel() = default;

public:
    QString caption() const override {return QStringLiteral("Minimum");}
    bool captionVisible() const override {return true;}
    bool portCaptionVisible(PortType, PortIndex) const override {return true;}

    QString portCaption(PortType portType, PortIndex portIndex) const override;
    QString name() const override {return QStringLiteral("Minimum");}
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
