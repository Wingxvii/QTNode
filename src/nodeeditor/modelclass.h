#ifndef MODELCLASS_H
#define MODELCLASS_H

//Main includes needed by all classes
#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//Out Port Data Types
#include "DataTypes/videographdata.h"

//QT widgets
#include <QLabel>
#include <QGridLayout>

//OpenCV and any other includes
#include "opencv2/imgproc.hpp"

//required USING Statements for node editor
using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class ModelClass: public NodeDataModel
{   Q_OBJECT

public: //constructor and destructor
    ModelClass();
    virtual ~ModelClass(){};

public: //required classes
    QString caption() const override{
        return QStringLiteral("Display Cascades");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Display Cascades");
    }
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

public slots: //required slots
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

public: //multithread

    void multiThreadedProcess();

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressBar;
public slots:
    void multiThreadedFinished();

private: //ports
    std::shared_ptr<VideoGraphData> videoIn;

private: //locals
    int x = 1;

private: //UI
    QGridLayout *layout;

};

#endif // MODELCLASS_H
