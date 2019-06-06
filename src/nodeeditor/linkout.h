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

    NodeValidationState validationState() const override;
    QString validationMessage() const override;
    bool resizable() const override{return false;}

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

    std::shared_ptr<CalibData> dataOut;

private: //locals
    QString index = "";

private: //UI

    QGridLayout *layout;
    QLabel *statusLabel;
    QLabel *indexLabel;
    QLineEdit* indexInput;

};


#endif // LINKOUT_H
