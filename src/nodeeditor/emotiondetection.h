#ifndef EMOTIONDETECTION_H
#define EMOTIONDETECTION_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"
#include "DataTypes/emotiondata.h"

//QT widgets
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QListWidget>
#include <QPushButton>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facerec.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class EmotionDetection : public NodeDataModel
{   Q_OBJECT

public:
    EmotionDetection();
    virtual ~EmotionDetection(){};


    QString caption() const override{
        return QStringLiteral("Detect Emotions");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Detect Emotions");
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

public slots:
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

    void getModel();
    void selectClasses();

private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<EmotionData> dataOut;

private: //locals
    cv::Ptr<cv::face::FisherFaceRecognizer> fishface;
    std::string classesFile;
    std::vector<std::string> classes;


private: //UI
    QGridLayout *layout;
    QPushButton *selectModel;
    QLabel *modelSelection;

    QPushButton *fileClasses;
    QLabel *fileClassesLabel;


};
#endif // EMOTIONDETECTION_H
