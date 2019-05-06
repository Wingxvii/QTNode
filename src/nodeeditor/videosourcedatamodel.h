#ifndef VIDEOSOURCEDATAMODEL_H
#define VIDEOSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QComboBox>

#include <nodes/NodeDataModel>
#include <QPushButton>

#include <iostream>

#include "QLabel"
#include "videographdata.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class VideoSourceDataModel : public NodeDataModel
{
    Q_OBJECT
public:
    VideoSourceDataModel();
    virtual ~VideoSourceDataModel();

    QString caption() const override
    {
        return QStringLiteral("Video Source");
    }

    bool captionVisible() const override
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Video Source");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, int) override {}
    QWidget *embeddedWidget() override {return button; }

public slots:
    void chooseVideo();

private:
    std::shared_ptr<VideoGraphData> _data;
    QPushButton* button;
};

#endif // VIDEOSOURCEDATAMODEL_H
