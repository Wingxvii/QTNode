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

#include "nodeeditor/DataTypes/calibdata.h"
#include "nodeeditor/DataTypes/imagedata.h"
#include "nodeeditor/DataTypes/pointdata.h"
#include "nodeeditor/DataTypes/pointsdata.h"
#include "nodeeditor/DataTypes/videographdata.h"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

enum dataTypes{
    NONE = -1,
   CALIBDATATYPE = 0,
    IMAGEDATATYPE = 1,
    POINTDATATYPE = 2,
    POINTSDATATYPE = 3,
    VIDEODATATYPE = 4

};

class LinkIn : public NodeDataModel{
Q_OBJECT

public:
    LinkIn();
    virtual ~LinkIn() {}

    QString caption()const override{
        return QStringLiteral("Linker Input");
    }

    bool captionVisible(){
        return true;
    }
    QString name()const override{
        return QStringLiteral("Linker Input");
    }

public:
    unsigned int nPorts(PortType PortType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override{return NULL;}
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


private: //ports

    std::shared_ptr<QtNodes::NodeData> dataIn;

private: //locals
    dataTypes inDataType = NONE;
    int index;

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;
    QLabel *typeSelectorLabel;
    QComboBox *typeSelection;

    QRegExpValidator* intPos;

};

#endif // LINKIN_H
