#ifndef GRAPHSOURCEDATAMODEL_H
#define GRAPHSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QComboBox>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"

#include <iostream>


class NodeGraphData;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class GraphSourceDataModel : public NodeDataModel
{
    Q_OBJECT
public:
    GraphSourceDataModel();
    virtual ~GraphSourceDataModel();

    QString caption() const override
    {
        return QStringLiteral("Graph Source");
    }

    bool captionVisible() const override
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("GraphSource");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, int) override { }
    QWidget *embeddedWidget() override{ return _comboBox; }

private slots:
    void onSelectionChanged(QString const index);
    void updateData();

private:
    GraphDataConnector* info;
    std::shared_ptr<NodeGraphData> _data;
    QComboBox* _comboBox;

};

#endif // GRAPHSOURCEDATAMODEL_H
