#ifndef NUMBERSOURCEDATAMODEL_H
#define NUMBERSOURCEDATAMODEL_H

#include <QtWidgets/QLineEdit>
#include "nodes/NodeDataModel"

using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;

class DecimalData;

class NumberSourceDataModel : public NodeDataModel
{

public:
    NumberSourceDataModel();
    virtual ~NumberSourceDataModel(){}

    //these are the in line overrides
public:
    QString caption() const {return QStringLiteral("Number Source");}
    bool captionVisible() const {return false;}
    QString name() const {return QStringLiteral("NumberSource");}

public:
    QJsonObject save() const ;
    void restore(QJsonObject const &p) ;

public:
    unsigned int nPorts(PortType portType) const ;
    NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const;
    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) ;
    void setInData(std::shared_ptr<NodeData>, int)  { }
    QWidget *embeddedWidget()  {return _lineEdit;}

private slots:

    void onTextEdited(QString const &string);

private:

    std::shared_ptr<DecimalData> _number;
    QLineEdit *_lineEdit;

};

#endif // NUMBERSOURCEDATAMODEL_H
