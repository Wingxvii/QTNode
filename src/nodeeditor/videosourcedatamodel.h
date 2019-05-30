#ifndef VIDEOSOURCEDATAMODEL_H
#define VIDEOSOURCEDATAMODEL_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QProgressBar>

#include <nodes/NodeDataModel>
#include <QPushButton>
#include <QtConcurrent/QtConcurrent>
#include <iostream>

#include "QLabel"
#include "DataTypes/videographdata.h"

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

public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

public: //multithread

    void multiThreadedProcess(cv::VideoCapture capture);

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressBar;
public slots:
    void multiThreadedFinished();

private: //UI
    QVBoxLayout *layout;
    QPushButton *button;

private: //Ports
    std::shared_ptr<VideoGraphData> _data;
};

#endif // VIDEOSOURCEDATAMODEL_H
