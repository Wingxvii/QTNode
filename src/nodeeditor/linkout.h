#ifndef LINKOUT_H
#define LINKOUT_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"


#include "analyzer/linkmanager.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include <QComboBox>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class CalibLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    CalibLinkOut();
    virtual ~CalibLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Calib Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Calib Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<CalibData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class ImageLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    ImageLinkOut();
    virtual ~ImageLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Image Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Image Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<ImageData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class PointLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    PointLinkOut();
    virtual ~PointLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Point Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Point Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<PointData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class PointsLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    PointsLinkOut();
    virtual ~PointsLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Points Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Points Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<PointsData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class VideoLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    VideoLinkOut();
    virtual ~VideoLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Video Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Video Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<VideoGraphData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class DetectionLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    DetectionLinkOut();
    virtual ~DetectionLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Detection Boxes Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Detection Boxes Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<DetectionBoxesData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

class EmotionLinkOut : public NodeDataModel{
    Q_OBJECT

public:
    EmotionLinkOut();
    virtual ~EmotionLinkOut(){}

    QString caption()const override{
        return QStringLiteral("Emotion Data Linker Output");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Emotion Data Linker Output");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override{}

    bool resizable() const override{return false;}

private:
    QJsonObject save() const override;
    virtual void restore(QJsonObject const &) override;


public slots:
    void processData() override;
    void preCheck() override;

    void ShowContextMenu(const QPoint &pos) override;

    void activate(){active = true;preCheck();window->setStyleSheet("");}
    void deactivate(){active = false;window->setStyleSheet("background-color:rgb(200,200,200);");}


private: //ports

    std::shared_ptr<EmotionData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};

#endif // LINKOUT_H
