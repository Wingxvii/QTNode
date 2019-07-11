#ifndef CONVERTCOLOR_H
#define CONVERTCOLOR_H

#include <QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include <QtConcurrent/QtConcurrent>

//data types
#include "DataTypes/videographdata.h"

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

class convertColor : public NodeDataModel{
    Q_OBJECT
public:
    convertColor();
    ~convertColor(){}

    QString caption() const override{
        return QStringLiteral("Color Conversior");
    }

    bool captionVisible(){
        return false;
    }
    QString name()const override{
        return QStringLiteral("Color Convertor");
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

    void addCodes();

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
    cv::ColorConversionCodes code = (cv::ColorConversionCodes)0;



private:
    QGridLayout *layout;
    QListWidget *codeSelection;

    QRegExpValidator* intPos;

};

#endif // CONVERTCOLOR_H
