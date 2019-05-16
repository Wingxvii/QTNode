#ifndef VIDEODISPLAY_H
#define VIDEODISPLAY_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QComboBox>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "DataTypes/videographdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QRegExpValidator>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class VideoDisplay : public NodeDataModel
{
    Q_OBJECT
public:
    VideoDisplay();
    virtual ~VideoDisplay() {}

    QString caption() const override
    {
        return QStringLiteral("Video Display");
    }

    bool captionVisible()
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Video Display");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void processData() override;
    void preCheck() override;
    void saveFrameRate();

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private:
    std::shared_ptr<VideoGraphData> _data;
    QFormLayout* formLayout;

    QPushButton* button;
    QLabel* framerateText;
    QLineEdit* framerateEdit;

    QRegExpValidator* intPos;

    float frameRate = 0;

};

#endif // VIDEODISPLAY_H
