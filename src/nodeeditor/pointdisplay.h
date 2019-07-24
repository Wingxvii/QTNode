#ifndef POINTDISPLAY_H
#define POINTDISPLAY_H

#include <QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/pointdata.h"
#include "DataTypes/imagedata.h"

//QT widgets
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>


using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class PointDisplay : public NodeDataModel{
    Q_OBJECT
public:
    PointDisplay();
    ~PointDisplay(){}

    QString caption() const override{
        return QStringLiteral("Point Display");
    }

    bool captionVisible(){
        return false;
    }
    QString name()const override{
        return QStringLiteral("Point Display");
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

private: //ports
    std::shared_ptr<ImageData> imageIn;
    std::shared_ptr<PointData> pointIn;
    std::shared_ptr<ImageData> imageOut;

private: //UI
    QGridLayout *layout;

    std::vector<cv::Scalar> colors;

};

#endif // POINTSDISPLAY_H
