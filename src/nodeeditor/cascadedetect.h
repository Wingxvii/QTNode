#ifndef CASCADEDETECT_H
#define CASCADEDETECT_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"
#include "DataTypes/DetectionBoxesData.h"

//QT widgets
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QRegExpValidator>
#include <QListWidget>


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class CascadeDetect : public NodeDataModel
{   Q_OBJECT

public:
    CascadeDetect();
    virtual ~CascadeDetect(){};


    QString caption() const override{
        return QStringLiteral("Detect Cascades");
    }
    bool captionVisible(){
        return false;
    }
    QString name()const override
    {
        return QStringLiteral("Detect Cascades");
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

    void selectCascade(int);
    void setupCascades();


private: //ports
    std::shared_ptr<VideoGraphData> videoIn;
    std::shared_ptr<DetectionBoxesData> dataOut;

private: //locals
    cv::CascadeClassifier cascade1;
    double scale = 1;
    int selectedRow = -1;
    float scaleFactor = 1.2f;
    int neighbors = 5;

private: //UI
    QGridLayout *layout;
    QLineEdit *scaleInput;
    QLabel *scaleDisplay;
    QListWidget *cascadeSelection;

    QLabel* scaleFactorLabel;
    QLineEdit* scaleFactorInput;

    QLabel* neigborsLabel;
    QLineEdit* neighborsInput;

    QDoubleValidator* doublepos;
    QRegExpValidator* intPos;
};


#endif // CASCADEDETECT_H
