#include "additive.h"

#include <QtGui/QDoubleValidator>
#include "nodeeditor/nodegraphdata.h"
#include "filterable/graphstats.h"
#include "QLineEdit"

Additive::Additive()
{
}

QJsonObject Additive::save() const
{
    return QJsonObject();
}

unsigned int Additive::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 2;
        break;

    case PortType::Out:
        result = 1;

    default:
        break;
    }

    return result;
}

NodeDataType Additive::dataType(QtNodes::PortType portType, QtNodes::PortIndex PortIndex) const
{
    return NodeGraphData().type();
}

std::shared_ptr<NodeData> Additive::outData(QtNodes::PortIndex)
{
    return _result;
}

void Additive::setInData(std::shared_ptr<NodeData> data, PortIndex index)
{
    auto graphData = std::dynamic_pointer_cast<NodeGraphData>(data);
    if (index == 0)
    {
        _n1 = graphData;
    }
    else if (index == 1)
    {
        _n2 = graphData;
    }
    calculateResult();

}

void Additive::calculateResult()
{
    auto g1 = _n1.lock();
    auto g2 = _n2.lock();

    if (g1 && g2)
    {
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();

        std::map<float, float> finalSet = g1->data();
        std::map<float, float> compareSet = g2->data();
        auto kt = finalSet.begin();
        int count = 0;
        while (kt != finalSet.end())
        {
            count++;
            //if it doesnt exist erase the data
            if (compareSet.find(kt->first) == compareSet.end())
            {
                kt = finalSet.erase(kt);
            }
            else if (compareSet[kt->first] != kt->second)
            {
                kt = finalSet.erase(kt);
                LOG_CAMPBELL() << " testing ";
            }
            else
            {
                ++kt;
            }
        }
        _result = std::make_shared<NodeGraphData>(finalSet);
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

QWidget* Additive::embeddedWidget()
{
    return nullptr;
}

NodeValidationState Additive::validationState() const
{
    return modelValidationState;
}

QString Additive::validationMessage() const
{
    return modelValidationError;
}
