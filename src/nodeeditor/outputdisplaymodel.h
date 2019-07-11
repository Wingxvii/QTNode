#ifndef OUTPUTDISPLAYMODEL_H
#define OUTPUTDISPLAYMODEL_H

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

class OutputDisplayModel : public NodeDataModel
{
    Q_OBJECT
public:
    OutputDisplayModel();
    virtual ~OutputDisplayModel() {}

public:
    QString caption() const override {return QStringLiteral("Output Graph");}
    bool captionVisible() const override {return false;}
    QString name() const override {return QStringLiteral("Output Graph");}

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> data, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

private slots:
    void onCreateEvents();

signals:
    void sendCreateEvents(std::vector<float>);
    void sendDeleteEvents(std::vector<float>);

private:
    std::vector<float> calculateEvents();

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    std::map<float, float> _data;
    bool eventsCreated;

    GraphStats stats;
    //LineGraph* _lineGraph;
    QPushButton *createEvents;
    QVBoxLayout* layout;

};

#endif // GRAPHDISPLAYDATAMODEL_H
