#include "OutputDisplayModel.h"

#include "nodeeditor/DataTypes/nodegraphdata.h"

OutputDisplayModel::OutputDisplayModel()
{
    layout = new QVBoxLayout();
    window = new QWidget();
    window->setLayout(layout);

    //_lineGraph = new LineGraph();
    createEvents = new QPushButton("create events");

    layout->addWidget(createEvents, 0);
    //layout->addWidget(_lineGraph, 1);

    connect(createEvents, SIGNAL(clicked(bool)), this, SLOT(onCreateEvents()));

    stats = GraphStats();
    eventsCreated = false;
}

void OutputDisplayModel::onCreateEvents()
{
    if (_data.size() > 0 && modelValidationState == NodeValidationState::Valid && eventsCreated == false)
    {
        emit sendCreateEvents(calculateEvents());
        eventsCreated = true;
    }
}

std::vector<float> OutputDisplayModel::calculateEvents()
{
    std::vector<float> points;
    for (auto it : _data)
    {
        points.push_back((it.first - stats.xMin) / 1000);
        LOG_CAMPBELL() << "events: " + QString::number((it.first - stats.xMin) / 1000);
    }
    return points;
}

unsigned int OutputDisplayModel::nPorts(QtNodes::PortType portType) const
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

NodeDataType OutputDisplayModel::dataType(PortType, PortIndex) const
{
    return NodeGraphData().type();
}

std::shared_ptr<NodeData> OutputDisplayModel::outData(QtNodes::PortIndex port)
{
    std::shared_ptr<NodeData> ptr;
    return ptr;
}

void OutputDisplayModel::setInData(std::shared_ptr<QtNodes::NodeData> data, int)
{
    auto graphData = std::dynamic_pointer_cast<NodeGraphData>(data);

    if (graphData)
    {

        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();

        if (eventsCreated == true)
        {
            if (eventsCreated == true)
            {
                eventsCreated = false;
                emit sendDeleteEvents(calculateEvents());
            }
        }

        _data = graphData->data();
        stats = GraphStats();
        for (auto point : _data)
        {
            stats.addData(point.first, point.second);
        }

        //_lineGraph->clearPixmap();
        //_lineGraph->setGraphStats(stats);
        //_lineGraph->updatePixmap(_data);
        //_lineGraph->update();
    }
    else
    {
        LOG_CAMPBELL() << "error non valid input";

        if (eventsCreated == true)
        {
            eventsCreated = false;
            emit sendDeleteEvents(calculateEvents());
        }

        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        stats = GraphStats();
        //_lineGraph->clearPixmap();
        //_lineGraph->setGraphStats(stats);
        //_lineGraph->updatePixmap(std::map<float, float>());
        //_lineGraph->update();


    }
    //_lineGraph->adjustSize();
    window->adjustSize();
}

NodeValidationState OutputDisplayModel::validationState() const
{
    return modelValidationState;
}

QString OutputDisplayModel::validationMessage() const
{
    return modelValidationError;
}
