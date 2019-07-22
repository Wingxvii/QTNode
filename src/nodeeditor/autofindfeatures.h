#ifndef AUTOFINDFEATURES_H
#define AUTOFINDFEATURES_H

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

class AutoFindFeatures: public NodeDataModel{
    Q_OBJECT

public:
    AutoFindFeatures();
    virtual ~AutoFindFeatures(){}

    QString caption() const override{
        return QStringLiteral("Automatically Find Features");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Automatically Find Features");
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

    void onGenImage();
    void onDelete();
    void onAdd(int, int, QString);
    void onEdit();
    void onRegen();
    void onClear();
    void onAddInPoints();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    //QJsonObject save() const override;
    //virtual void restore(QJsonObject const &) override;
private: //port values
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<DetectionBoxesData> pointsIn;
    std::shared_ptr<ImageData> imageOut;
    std::shared_ptr<PointData> pointsOut;

    std::shared_ptr<ImageData> displayImage;

private: //UI
    QGridLayout *layout;

    QLabel *displayCacheIndexLabel;
    QLineEdit *displayCacheIndex;
    QCheckBox *generateImage;

    QLabel *selectFrameLabel;
    QLineEdit *selectFrame;

    QListWidget *allPoints;

    QPushButton *deleteButton;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *reGenButton;
    QPushButton *addInPoints;
    QPushButton *clearButton;
    //new ongenimage button

    QLabel *maxCornersLabel;
    QLineEdit *maxCorners;
    QLabel *qualityLevelLabel;
    QLineEdit *qualityLevel;
    QLabel *minDistanceLabel;
    QLineEdit *minDistance;
    QLabel *blockSizeLabel;
    QLineEdit *blockSize;

    AddFeature *addWindow;

    QRegExpValidator* intPos;
    QDoubleValidator* doublePos;


private: //locals
    int frameSelected= -1;
    int MaxCorners = -1;
    double QualityLevel = -1;
    double MinDistance = -1;
    int BlockSize = -1;
    QString DisplayCacheIndex;

    bool isReady= false;

};

#endif // AUTOFINDFEATURES_H
