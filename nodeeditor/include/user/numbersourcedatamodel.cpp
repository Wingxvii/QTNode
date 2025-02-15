#include "numbersourcedatamodel.h"

#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>

#include "nodes/decimaldata.h"

NumberSourceDataModel::NumberSourceDataModel() : _lineEdit(new QLineEdit)
{
    _lineEdit->setValidator(new QDoubleValidator);
    _lineEdit->setMaximumSize(_lineEdit->sizeHint());

    connect(_lineEdit, & QLineEdit::textChanged, this, &NumberSourceDataModel::onTextEdited);
    _lineEdit->setText("0.0");
}

QJsonObject NumberSourceDataModel::save() const
{
    QJsonObject modelJson; //= NodeDataModel::save();

    if (_number)
    {
        modelJson["number"] = QString::number(_number->number());
    }

    return modelJson;
}

void NumberSourceDataModel::restore(const QJsonObject &p)
{
    QJsonValue v = p["number"];

    if (!v.isUndefined())
    {
        QString strNum = v.toString();

        bool ok;
        double d = strNum.toDouble(&ok);
        if (ok)
        {
            _number = std::make_shared<DecimalData>(d);
            _lineEdit->setText(strNum);
        }
    }
}


unsigned int NumberSourceDataModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 0;
        break;

    case PortType::Out:
        result = 1;
        break;

    default:
        break;
    }
    return result;
}

void NumberSourceDataModel::onTextEdited(const QString &string)
{
    Q_UNUSED(string);

    bool ok = false;
    double number = _lineEdit->text().toDouble(&ok);

    if (ok)
    {
        _number = std::make_shared<DecimalData>(number);
        //emit dataUpdated(0);
    }
    else
    {
       // emit dataInvalidated(0);
    }
}

NodeDataType NumberSourceDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
     return DecimalData().type();
}

std::shared_ptr<NodeData> NumberSourceDataModel::outData(PortIndex)
{
    return _number;
}
