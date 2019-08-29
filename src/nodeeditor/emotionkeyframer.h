#ifndef EMOTIONKEYFRAMER_H
#define EMOTIONKEYFRAMER_H

#include <QtCore/QObject>

#include <nodes/NodeDataModel>
#include "analyzer/graphdataconnector.h"
#include "analyzer/linkmanager.h"

#include <QtConcurrent/QtConcurrent>

#include <iostream>
#include <QLabel>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QRegExpValidator>
#include <QPushButton>
#include <QDoubleValidator>
#include <QComboBox>
#include <QListWidget>

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class EmotionKeyframer: public NodeDataModel{
    Q_OBJECT

public:
    EmotionKeyframer();
    ~EmotionKeyframer(){}

    QString caption() const override{
        return QStringLiteral("Find Emotion Keyframes");
    }

    bool captionVisible(){
        return false;
    }

    QString name()const override
    {
        return QStringLiteral("Find Emotion Keyframes");
    }

public:

    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override;

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override {return false;}

public slots:
    //this is our work function
    void processData() override;
    //this is our check function
    void preCheck() override;

    //context menu slots
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

    void saveToFile();

    void onDelete();
    void onClear();

private:
    NodeValidationState modelValidationState = NodeValidationState::Warning;
    QString modelValidationError = QStringLiteral("Missing or incorrect inputs");

private: //port values
    std::shared_ptr<EmotionData> dataIn;
    std::shared_ptr<EmotionData> dataOut;

private: //UI
    QGridLayout *layout;
    QComboBox *findMethodSelector;
    QComboBox *labelSelector;
    QComboBox *saveTypeSelector;

    QLabel* thresholdLabel;
    QLineEdit *threshold;

    QListWidget *keyframes;

    QPushButton *saveData;
    QPushButton *applyMethod;

    QPushButton *deleteButton;
    QPushButton *clearButton;

    QRegExpValidator* intPos;

private: //locals
    int Threshold = -1;


};

#endif // EMOTIONKEYFRAMER_H
