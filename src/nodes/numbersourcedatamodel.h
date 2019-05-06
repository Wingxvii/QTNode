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
    Q_OBJECT

public:
    NumberSourceDataModel();
    virtual ~NumberSourceDataModel(){}

    //these are the in line overrides
public:
    QString caption() const override {return QStringLiteral("Number Source");}
    bool captionVisible() const override {return false;}
    QString name() const override {return QStringLiteral("NumberSource");}

public:
    QJsonObject save() const override;
    void restore(QJsonObject const &p) override;

public:
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const;
    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, int) override { }
    QWidget *embeddedWidget() override {return _lineEdit;}

private slots:

    void onTextEdited(QString const &string);

private:
    std::shared_ptr<DecimalData> _number;

    QLineEdit *_lineEdit;

};

#endif // NUMBERSOURCEDATAMODEL_H
