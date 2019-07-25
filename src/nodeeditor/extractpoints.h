#ifndef EXTRACTPOINTS_H
#define EXTRACTPOINTS_H


#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "DataTypes/pointsdata.h"
#include "DataTypes/pointdata.h"

#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>

//opencv
#include "opencv2\core.hpp"
#include "opencv2\imgcodecs.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\calib3d.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class ExtractPoints : public NodeDataModel{
    Q_OBJECT
    public:
    ExtractPoints();
    ~ExtractPoints(){}

    QString caption() const override{
        return QStringLiteral("Points Iterator");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Points Iterator");
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
    void iterateForward();
    void iterateBackward();
    void gotoFrame();
    void processData() override;
    void preCheck() override;
    void updateUI();

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

private: //port values
    std::shared_ptr<PointsData> pointsIn;
    std::shared_ptr<PointData> pointOut;

private: //locals
    int currFrame = 0;
    int totalFrames = 0;

private: //UI
    QGridLayout *layout;
    QPushButton* forward;
    QPushButton* backward;
    QPushButton* frameTo;
    QLabel* frameNumber;
    QLineEdit* frameSelector;
    QLabel *totalFramesDisplay;
    QLabel *totalFramesLabel;

    QRegExpValidator* intValid;
};

#endif // EXTRACTPOINTS_H
