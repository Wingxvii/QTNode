#ifndef BUFFERNODE_H
#define BUFFERNODE_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <iostream>

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class BufferNode: public NodeDataModel{
    Q_OBJECT

public:
    BufferNode();
    virtual ~BufferNode(){}

    QString caption() const override{
        return QStringLiteral("Node Buffer");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Node Buffer");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

public slots:
    void processData();
    void preCheck();

    void ShowContextMenu(const QPoint &pos) override;
    void activate();
    void deactivate();

private: //ports
    std::shared_ptr<NodeData> inPort1;
    std::shared_ptr<NodeData> inPort2;
    std::shared_ptr<NodeData> inPort3;

    std::shared_ptr<NodeData> outPort1;
    std::shared_ptr<NodeData> outPort2;
    std::shared_ptr<NodeData> outPort3;

private:
    QGridLayout *layout;
    QLabel *status;
    QPushButton *activateButton;
    QPushButton *deactivateButton;




};


#endif // BUFFERNODE_H
