#ifndef LINKIN_H
#define LINKIN_H

#include <QtCore/QObject>
#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"


#include "analyzer/linkmanager.h"

#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QRegExpValidator>
#include <QComboBox>
#include <QPushButton>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;


class CalibLinkIn : public NodeDataModel{
Q_OBJECT

public:
    CalibLinkIn();
    virtual ~CalibLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Calib Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Calib Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<CalibData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;
};


class ImageLinkIn : public NodeDataModel{
Q_OBJECT

public:
    ImageLinkIn();
    virtual ~ImageLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Image Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Image Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<ImageData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};


class PointLinkIn : public NodeDataModel{
Q_OBJECT

public:
    PointLinkIn();
    virtual ~PointLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Point Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Point Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<PointData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};


class PointsLinkIn : public NodeDataModel{
Q_OBJECT

public:
    PointsLinkIn();
    virtual ~PointsLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Points Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Points Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<PointsData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};


class VideoLinkIn : public NodeDataModel{
Q_OBJECT

public:
    VideoLinkIn();
    virtual ~VideoLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Video Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Video Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<VideoGraphData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};



class DetectionLinkIn : public NodeDataModel{
Q_OBJECT

public:
    DetectionLinkIn();
    virtual ~DetectionLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Detection Boxes Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Detection Boxes Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<DetectionBoxesData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};

class EmotionLinkIn : public NodeDataModel{
Q_OBJECT

public:
    EmotionLinkIn();
    virtual ~EmotionLinkIn() {}

    QString caption()const override{
        return QStringLiteral("Emotion Data Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Emotion Data Linker Input");
    }

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<EmotionData> dataIn;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QPushButton* send;

};


#endif // LINKIN_H
