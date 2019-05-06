#include "minmodel.h"

#include <QtGui/QDoubleValidator>
#include "nodeeditor/nodegraphdata.h"
#include "filterable/graphstats.h"
#include "QLineEdit"

MinModel::MinModel()
{
    _lineEdit = new QLineEdit();
    _lineEdit->setValidator(new QDoubleValidator(0, 1, 5));
    _lineEdit->setMaximumSize(_lineEdit->sizeHint());

    connect(_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTextEdited()));

    _lineEdit->setText("0.0");
}


QString MinModel::portCaption(PortType portType, PortIndex portIndex) const
{
    switch (portType)
    {
    case PortType::In:
        if (portIndex == 0)
        {
            return QStringLiteral("input");
        }
        break;
    case PortType::Out:
        return QStringLiteral("Result");
    default:
        break;
    }
    return QString();
}

QJsonObject MinModel::save() const
{
    return QJsonObject();
}

unsigned int MinModel::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 1;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}

NodeDataType MinModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex PortIndex) const
{
    return NodeGraphData().type();
}

std::shared_ptr<NodeData> MinModel::outData(QtNodes::PortIndex)
{
    return _result;
}

void MinModel::setInData(std::shared_ptr<NodeData> data, PortIndex index)
{
    auto graphData = std::dynamic_pointer_cast<NodeGraphData>(data);
    _data = graphData;
    calculateResult();
}

void MinModel::onTextEdited()
{
    calculateResult();
}

void MinModel::calculateResult()
{
    auto g1 = _data.lock();

    bool okay;
    double percent = _lineEdit->text().toDouble(&okay);
    LOG_CAMPBELL() << "line edit " + QString::number(percent);
    if (g1 && okay)
    {
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
        double xPoint1 = INFINITY, xPoint2 = INFINITY;
        double yPoint1 = INFINITY, yPoint2 = INFINITY;
        bool connected = false;
        bool lastState = false;

        std::map<float, float> points;
        GraphStats stats;

        for (auto it : g1->data())
        {
            stats.addData(it.first, it.second);
        }
        for (auto it : g1->data())
        {
            double xTemp = it.first;
            double yTemp = it.second;

            xPoint2 = xPoint1;
            yPoint2 = yPoint1;

            xPoint1 = xTemp;
            yPoint1 = yTemp;
            if (yPoint2 != INFINITY && yPoint1 != INFINITY)
            {
                if ((yPoint1 - stats.yMin) / stats.yRange > percent)
                {
                    lastState = connected;
                    connected = true;
                    points[xPoint1] = yPoint1;
                }
                else
                {
                    lastState = connected;
                    connected = false;
                }
                if (connected != lastState)
                {
                    //find where it ends
                    double y = (percent * stats.yRange) + stats.yMin; //good
                    double m = (yPoint1 - yPoint2) / (xPoint1 - xPoint2); // bad?
                    double b = yPoint2 - (xPoint2 * m);
                    double x = (y - b) / m;
                    if (x >= xPoint2 && x <= xPoint1)
                    {
                        points[x] = y;
                    }
                    else
                    {
                        LOG_CAMPBELL() << "point failed to calculate";
                    }

                    //add a point below min range to let our graph now not to connect
                    //points.second->filtered[x + ((xPoint1 - x)/2)] = yMin - 1;
                }
            }

        }
        _result = std::make_shared<NodeGraphData>(points);
    }
    else
    {
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        _result.reset();
    }
    PortIndex const outPortIndex = 0;
    emit dataUpdated(outPortIndex);
}

QWidget* MinModel::embeddedWidget()
{
    return _lineEdit;
}

NodeValidationState MinModel::validationState() const
{
    return modelValidationState;
}

QString MinModel::validationMessage() const
{
    return modelValidationError;
}
