#ifndef CHECKERBOARDPOINTSBUFFER_H
#define CHECKERBOARDPOINTSBUFFER_H

//important includes
#include <nodes/NodeDataModel>
#include <QtCore/QObject>
#include "analyzer/graphdataconnector.h"
#include "Logger.h"

//ports
#include "pointsdata.h"
#include "pointdata.h"

//widgets
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>

using QtNodes::PortType;
using QtNodes::NodeDataModel;
using QtNodes::PortIndex;
using QtNodes::NodeValidationState;
using QtNodes::NodeData;
using QtNodes::NodeDataType;

class CheckerboardPointsBuffer : public NodeDataModel{
Q_OBJECT
public:
    CheckerboardPointsBuffer();
    virtual ~CheckerboardPointsBuffer();

    QString caption() const override{
        return QStringLiteral("Checkerboard Points Buffer");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Checkerboard Points Buffer");
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
    void deleteData();
    void addData();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<PointsData> pointsOut;
    std::shared_ptr<PointData> pointIn;

private: //UI
    QWidget *window;
    QGridLayout *layout;
    QPushButton *deleteButton;
    QPushButton *addButton;
    QListWidget *dataList;
    int numberOfPoints = 0;

};


#endif // CHECKERBOARDPOINTSBUFFER_H
