#ifndef AUTOFRAMEITERATOR_H
#define AUTOFRAMEITERATOR_H

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
#include <QProgressBar>
#include <QCheckBox>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class AutoFrameIterator : public NodeDataModel{
    Q_OBJECT
public:
    AutoFrameIterator();
    virtual ~AutoFrameIterator(){}

    QString caption() const override{
        return QStringLiteral("Automatic Frame Iterator");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Automatic Frame Iterator");
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
    //calculates the values displayed. This is called whenever a value or input is updated
    void updateUI();

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
    std::shared_ptr<VideoGraphData> imagesOut;

private: //locals
    bool isReady;

    int totalFrames = 0;
    int projectedSamples = 0;
    int startFrame = 0;
    int endFrame = -1;
    int byPass = 1;

private: //UI
    QGridLayout *layout;

    QLabel *totalFramesDisplay;
    QLabel *totalFramesLabel;
    QLabel *startLabel;
    QLineEdit* startFrameInput;
    QLabel *endLabel;
    QLineEdit* endFrameInput;
    QLabel *byPassLabel;
    QLineEdit *byPassInput;
    QLabel *projectedSamplesLabel;
    QLabel *projectedSamplesDisplay;

    QRegExpValidator* intPos;

};
#endif // AUTOFRAMEITERATOR_H
