#ifndef DILATEIMAGE_H
#define DILATEIMAGE_H
#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "videographdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QComboBox>
#include <QFormLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class DilateImage : public NodeDataModel{
    Q_OBJECT


public:
    DilateImage();
    virtual ~DilateImage() {}

    QString caption() const override{
        return QStringLiteral("Image Dilation");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Image Dilation");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    QWidget* embeddedWidget() override {return window;}
    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    void startDilation();
    void changeShape();
    void changeSize();
private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<VideoGraphData> videoOut;
    QPushButton* button;
    QComboBox* shapeSelection;
    QComboBox* sizeSelecton;

    QFormLayout* layout;
    QWidget *window;
    QLabel *sizeLabel;
    QLabel *shapeLabel;

    int size = 8;
    cv::MorphShapes shape = cv::MorphShapes::MORPH_RECT;


};
#endif // DILATEIMAGE_H
