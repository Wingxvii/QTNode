#ifndef RESIZEVIDEONODE_H
#define RESIZEVIDEONODE_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"

//QT widgets
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QComboBox>
#include <QCheckBox>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class ResizeVideoNode : public NodeDataModel{
    Q_OBJECT
public:
    ResizeVideoNode();
    virtual ~ResizeVideoNode(){}

    QString caption() const override{
        return QStringLiteral("Resize and Rotate Video");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Resize and Rotate Video");
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

    void multiThreadedProcess();

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressBar;
public slots:
    void multiThreadedFinished();


private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<VideoGraphData> videoOut;

private: //locals
    bool isReady;
    double ResizeScaleX = -1;
    double ResizeScaleY = -1;
    int interpIndex = -1;
    double Angle = -1;
    bool Rotate;
    bool Resize;

private: //UI
    QGridLayout *layout;

    QLabel *resizeCheckLabel;
    QCheckBox* resize;
    QLabel *rotateCheckLabel;
    QCheckBox* rotate;

    QLabel *interpolationMethodLabel;
    QComboBox *interpolationMethod;

    QLabel *resizeLabelX;
    QLineEdit *resizeScaleX;
    QLabel *resizeLabelY;
    QLineEdit *resizeScaleY;

    QLabel* angleLabel;
    QLineEdit* angle;

    QDoubleValidator* doublePos;

};


#endif // RESIZERVIDEO_H
