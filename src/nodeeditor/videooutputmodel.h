#ifndef VIDEOOUTPUTMODEL_H
#define VIDEOOUTPUTMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <analyzer/linegraph.h>
#include <QPushButton>
#include <QGridLayout>
//<>><><<>><><<>//

#include <nodes/NodeDataModel>
#include "nodeeditor/DataTypes/videographdata.h"
#include <iostream>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class VideoOutputModel : public NodeDataModel
{
    Q_OBJECT
public:

    VideoOutputModel();
    virtual ~VideoOutputModel() {}

public:
    QString caption() const override {return QStringLiteral("Output Video");}
    bool captionVisible() const override {return false;}
    QString name() const override {return QStringLiteral("Output Video");}

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> data, int) override;
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void createEvents();

signals:
    void sendCreateEvents(std::vector<float>);
    void sendDeleteEvents(std::vector<float>);

private:

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    std::shared_ptr<VideoGraphData> _data;
    QPushButton* button;
    QGridLayout* layout;

    std::vector<float> events;


};

#endif // VIDEOOUTPUTMODEL_H
