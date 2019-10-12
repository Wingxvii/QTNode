#ifndef TRAINMOTIONMODELS_H
#define TRAINMOTIONMODELS_H

#include <QtCore/QObject>
#include <QtWidgets/QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QFileDialog>
#include <nodes/NodeDataModel>
#include <QPushButton>
#include <QtConcurrent/QtConcurrent>

#include "QLabel"
#include "DataTypes/videographdata.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facerec.hpp"
#include <random>
#include <ctime>

using namespace cv;
using namespace dnn;
using namespace std;

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


enum emotions {
    None = 0,
    Surprise = 1,
    Fear = 2,
    Disgust = 3,
    Happiness = 4,
    Sadness = 5,
    Anger = 6,
    Neutral = 7,
};

class TrainEmotionModel : public NodeDataModel
{
    Q_OBJECT
public:
    TrainEmotionModel();
    virtual ~TrainEmotionModel();

    QString caption() const override
    {
        return QStringLiteral("Train Emotion Models");
    }

    bool captionVisible() const override
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Train Emotion Models");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, int) override {}

public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;
    void activate(){active = true;window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

public: //multithread

    void multiThreadedProcess();

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressBar;
public slots:
    void multiThreadedFinished();
    //void testModel();

private: //UI
    QVBoxLayout *layout;
    QPushButton *button;
    QPushButton *modelSelection;

public:
    Ptr<face::FisherFaceRecognizer> fishface;
    Ptr<face::EigenFaceRecognizer> eigenface;
    Ptr<face::LBPHFaceRecognizer> lbphface;

    map<int, double> valuePercentages;
    int rateThreshold = 75;
    int coun = 0;

    int iterator = 0;
    vector<Mat> trainingData, testingData;
    vector<int> trainingLabels, testingLabels;

};

#endif // TRAINMOTIONMODELS_H
