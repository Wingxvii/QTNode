#ifndef DEEPIMAGECLASSIFICATION_H
#define DEEPIMAGECLASSIFICATION_H


#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"
#include "DataTypes/DetectionBoxesData.h"

//QT widgets
#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QListWidget>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QPushButton>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/dnn.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class DeepImageClassification: public NodeDataModel
{   Q_OBJECT

public:
    DeepImageClassification();
    virtual ~DeepImageClassification(){};
    QString caption() const override{
        return QStringLiteral("Deep Image Classification");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Deep Image Classification");
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

    void setupModels();
    void selectModels();

    void selectClasses();
    void selectConfig();
    void selectWeights();

private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<DetectionBoxesData> dataOut;

private: //locals
    std::string classesFile;
    cv::String modelConfig;
    cv::String modelWeights;

    std::vector<std::string> classes;

    cv::dnn::Net net;

    float confThreshold = 0.5; // Confidence threshold
    float nmsThreshold = 0.4;  // Non-maximum suppression threshold
    int inpWidth = 416;  // Width of network's input image
    int inpHeight = 416; // Height of network's input image

    //create neames
    std::vector<cv::String> names;

private: //UI
    QGridLayout *layout;
    QComboBox *modelSelection;
    QSlider *confidenceInput;
    QSlider *supressionInput;
    QLabel *confidenceLabel;
    QLabel *supressionLabel;

    QLabel *fileClassesLabel;
    QLabel *fileConfigLabel;
    QLabel *fileWeightsLabel;

    QPushButton *fileClasses;
    QPushButton *fileConfig;
    QPushButton *fileWeights;

    //size
    //conf threshold


};

#endif // DEEPIMAGECLASSIFICATION_H
