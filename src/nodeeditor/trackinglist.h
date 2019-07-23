#ifndef TRACKINGLIST_H
#define TRACKINGLIST_H
#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "analyzer/linkmanager.h"

#include <QtConcurrent/QtConcurrent>

#include <iostream>
#include <QLabel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRegExpValidator>
#include <QListWidget>
#include <QPushButton>
#include <QDoubleValidator>

#include "addfeature.h"

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"
#include "opencv2\videoio.hpp"
#include "opencv2\video.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class TrackingList: public NodeDataModel{
    Q_OBJECT
public:
    TrackingList();
    virtual ~TrackingList(){}

    QString caption() const override{
        return QStringLiteral("Edit Tracking List");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Edit Tracking List");
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
    //this is our work function
    void processData() override;
    //this is our check function
    void preCheck() override;

    //context menu slots
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

    void onDelete();
    void onAdd(int, int, QString);
    void onEditRecieve(int, int, int, QString);
    void onEdit();
    void onRegen();
    void onClear();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private:
    std::shared_ptr<PointData> pointsIn;
    std::shared_ptr<PointData> pointsOut;

private: //UI
    QGridLayout *layout;

    QListWidget *allPoints;
    QPushButton *deleteButton;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *reGenButton;
    QPushButton *clearButton;

    AddFeature *addWindow;

};

#endif // TRACKINGLIST_H
