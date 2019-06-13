#ifndef GETCORNERS_H
#define GETCORNERS_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "analyzer/linkmanager.h"

#include <QtConcurrent/QtConcurrent>
#include <QFutureIterator>

#include <QProgressBar>
#include <iostream>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class GetCorners : public NodeDataModel{
    Q_OBJECT


public:
    GetCorners();
    virtual ~GetCorners() {}

    QString caption() const override{
        return QStringLiteral("Find Corners");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Find Corners");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}


public: //multithread

    QFuture<std::vector<cv::Point2f>> funct;
    QFutureWatcher<void> functWatcher;
    QLabel *progressText;
    QProgressBar *progressBar;


public slots:
    void multiThreadedFinished();
    void multiThreadedUpdate();

public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //local variables
    int minimumSuccesses = 10;

private: //port values
    std::shared_ptr<VideoGraphData> imagesIn;
    std::shared_ptr<PointsData> cornersOut;
    std::shared_ptr<CalibData> dataIn;  //does not need to be cleared

private: //UI
    QGridLayout *layout;
    QLabel *successLabel;
    QLabel *successDisplay;
    QLabel *failLabel;
    QLabel *failDisplay;




};

#endif // GETCORNERS_H
