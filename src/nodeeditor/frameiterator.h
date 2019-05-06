#ifndef FRAMEITERATOR_H
#define FRAMEITERATOR_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "imagedata.h"
#include "videographdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class FrameIterator : public NodeDataModel{
Q_OBJECT
public:
    FrameIterator();
    virtual ~FrameIterator() {}

    QString caption() const override{
        return QStringLiteral("Frame Iterator");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Frame Iterator");
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
    void iterateForward();
    void iterateBackward();
    void selectFrame();
private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<ImageData> frameOut;

private: //locals
    int currFrame = 0;

private: //UI
    QWidget *window;
    QGridLayout *layout;
    QPushButton* forward;
    QPushButton* backward;
    QPushButton* frameTo;
    QLabel* frameNumber;
    QLineEdit* frameSelector;

    QRegExpValidator* intValid;
};


#endif // FRAMEITERATOR_H
