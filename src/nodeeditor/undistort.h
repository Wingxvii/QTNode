#ifndef UNDISTORT_H
#define UNDISTORT_H
#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "imagedata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class UnDistort : public NodeDataModel{
Q_OBJECT
public:
    UnDistort();
    virtual ~UnDistort() {}

    QString caption() const override{
        return QStringLiteral("UnDistort Image");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Undistort Image");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget* embeddedWidget() override {return button;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void startUnDistort();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<ImageData> cameraMatIn;
    std::shared_ptr<ImageData> distanceCoeffIn;
    std::shared_ptr<ImageData> imageIn;
    std::shared_ptr<ImageData> imageOut;
    QPushButton* button;


};

#endif // UNDISTORT_H
