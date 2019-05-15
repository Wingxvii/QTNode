#ifndef GETCORNERS_H
#define GETCORNERS_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "pointsdata.h"
#include "videographdata.h"
#include "calibdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class GetCorners : public NodeDataModel{
    Q_OBJECT


public:
    GetCorners();
    virtual ~GetCorners() {}

    QString caption() const override{
        return QStringLiteral("Find Corners");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Find Corners");
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
    void processData();
    void preCheck();
    void updateUI();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //local variables
    int successes = 0;
    int failures = 0;
    int minimumSuccesses = 10;

private: //port values
    std::shared_ptr<VideoGraphData> imagesIn;
    std::shared_ptr<PointsData> cornersOut;
    std::shared_ptr<CalibData> dataIn;  //does not need to be cleared

private: //UI
    QWidget *window;
    QGridLayout *layout;
    QLabel *successLabel;
    QLabel *successDisplay;
    QLabel *failLabel;
    QLabel *failDisplay;




};

#endif // GETCORNERS_H
