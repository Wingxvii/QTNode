#include "graphdisplaydatamodel.h"

#include "nodeeditor/nodegraphdata.h"

GraphDisplayDataModel::GraphDisplayDataModel()
{
    _lineGraph = new LineGraph();
    stats = GraphStats();
}


unsigned int GraphDisplayDataModel::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 0;
        break;
    default:
        break;
    }
    return result;
}

NodeDataType GraphDisplayDataModel::dataType(PortType, PortIndex) const
{
    return NodeGraphData().type();
}

std::shared_ptr<NodeData> GraphDisplayDataModel::outData(QtNodes::PortIndex port)
{
    std::shared_ptr<NodeData> ptr;
    return ptr;
}

void GraphDisplayDataModel::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    auto graphData = std::dynamic_pointer_cast<NodeGraphData>(data);

    if (graphData)
    {
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
        std::map<float, float> test = graphData->data();
        stats = GraphStats();
        for (auto point : test)
        {
            stats.addData(point.first, point.second);
        }
        _lineGraph->clearPixmap();
        _lineGraph->setGraphStats(stats);
        _lineGraph->updatePixmap(test);
        _lineGraph->update();
    }
    else
    {
        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        stats = GraphStats();
        _lineGraph->clearPixmap();
        _lineGraph->setGraphStats(stats);
        _lineGraph->updatePixmap(std::map<float, float>());
        _lineGraph->update();
    }
    _lineGraph->adjustSize();
}

NodeValidationState GraphDisplayDataModel::validationState() const
{
    return modelValidationState;
}

QString GraphDisplayDataModel::validationMessage() const
{
    return modelValidationError;
}
