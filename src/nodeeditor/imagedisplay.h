#ifndef IMAGEDISPLAY_H
#define IMAGEDISPLAY_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "DataTypes/imagedata.h"
#include <iostream>

#include <QGridLayout>

#include <QPushButton>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class ImageDisplay : public NodeDataModel
{
    Q_OBJECT

public:
    ImageDisplay();
    virtual ~ImageDisplay() {}

    QString caption() const override
    {
        return QStringLiteral("Image Display");
    }

    bool captionVisible()
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Image Display");
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

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private:
    std::shared_ptr<ImageData> _data;
    QGridLayout* layout;
    QPushButton* button;

};





#endif // IMAGEDISPLAY_H
