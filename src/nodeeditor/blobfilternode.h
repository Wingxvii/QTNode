#ifndef BLOBFILTERNODE_H
#define BLOBFILTERNODE_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
#include <QPushButton>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>
#include <iostream>
#include "videographdata.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class BlobFilterNode : public NodeDataModel
{
    Q_OBJECT
public:
    explicit BlobFilterNode();
    virtual ~BlobFilterNode() {}

public:
    QString caption() const override {return QStringLiteral("Blob filter");}
    bool captionVisible() const override {return true;}
    bool portCaptionVisible(PortType, PortIndex) {return true;}

    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QString name() const override {return QStringLiteral("Blob Filter");}

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget *embeddedWidget() override {return button;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;

private:

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    std::shared_ptr<VideoGraphData> _data;

    QPushButton* button;

};

#endif // BLOBFILTERNODE_H
