#ifndef EQUALIZEHIST_H
#define EQUALIZEHIST_H

#include <QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"

//QT widgets
#include <QLabel>
#include <QGridLayout>

//cv functions
#include "opencv2/imgproc.hpp"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class EqualizeHist : public NodeDataModel
{
    Q_OBJECT
public:
    EqualizeHist();
    ~EqualizeHist(){};

    QString caption() const override{
        return QStringLiteral("Equalize Histogram");
    }

    bool captionVisible(){
        return false;
    }
    QString name()const override{
        return QStringLiteral("Equalize Histogram");
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

private:
    QGridLayout *layout;

};

#endif // EQUALIZEHIST_H
