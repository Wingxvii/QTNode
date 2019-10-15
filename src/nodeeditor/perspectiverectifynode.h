#ifndef PERSPECTIVERECTIFYNODE_H
#define PERSPECTIVERECTIFYNODE_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"
#include "analyzer/linkmanager.h"

//QT widgets
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QCheckBox>
#include <QProgressBar>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class PerspectiveRectify : public NodeDataModel{
    Q_OBJECT
public:
    PerspectiveRectify();
    virtual ~PerspectiveRectify(){}

    QString caption() const override{
        return QStringLiteral("Perspective Rectify");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Perspective Rectify");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;

public slots:
    //iterates throught the video, saving frames to a image vector based on parameters
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

public: //multithread


    QFuture<cv::Mat> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressText;
    QProgressBar *progressBar;

    std::shared_ptr<ImageData> transformData;

public slots:
    void multiThreadedFinished();
    void multiThreadedUpdate();

private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<VideoGraphData> videoOut;

private: //locals
    bool isReady;

    int Corner1x = -1;
    int Corner2x = -1;
    int Corner3x = -1;
    int Corner4x = -1;
    int Corner1y = -1;
    int Corner2y = -1;
    int Corner3y = -1;
    int Corner4y = -1;

private: //UI
    QGridLayout *layout;

    QCheckBox *rectify;

    QLabel *coords1;
    QLabel *coords2;
    QLabel *coords3;
    QLabel *coords4;

    QLabel *comma1;
    QLabel *comma2;
    QLabel *comma3;
    QLabel *comma4;

    QLabel *close1;
    QLabel *close2;
    QLabel *close3;
    QLabel *close4;

    QLineEdit* corner1X;
    QLineEdit* corner1Y;
    QLineEdit* corner2X;
    QLineEdit* corner2Y;
    QLineEdit* corner3X;
    QLineEdit* corner3Y;
    QLineEdit* corner4X;
    QLineEdit* corner4Y;

    QRegExpValidator* intPos;

    float srcQ[4][2], dstQ[4][2];


};

#endif // PERSPECTIVERECTIFY_H
