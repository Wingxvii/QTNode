#include "graphsourcedatamodel.h"

#include "nodegraphdata.h"
#include "Logger.h"

GraphSourceDataModel::GraphSourceDataModel()
    : _comboBox(new QComboBox())
{
    info = &GraphDataConnector::getInstance();
    QStringList list = info->getNames();

    connect(info, SIGNAL(updateData()), this, SLOT(updateData()));

    for (int i = 0; i < list.size(); i++)
    {
        _comboBox->addItem(list.at(i));
    }
    onSelectionChanged(_comboBox->currentText());

    connect(_comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSelectionChanged(QString)));

}

void GraphSourceDataModel::updateData()
{
    _comboBox->clear();
    QStringList list = info->getNames();
    for (int i = 0; i < list.size(); i++)
    {
        _comboBox->addItem(list.at(i));
    }
    onSelectionChanged(_comboBox->currentText());
}

GraphSourceDataModel::~GraphSourceDataModel()
{

}


unsigned int GraphSourceDataModel::nPorts(PortType portType) const
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

NodeDataType GraphSourceDataModel::dataType(PortType, PortIndex) const
{
    return NodeGraphData().type();
}

void GraphSourceDataModel::onSelectionChanged(QString name)
{
    _data = std::make_shared<NodeGraphData>(info->getData(name));
    emit dataUpdated(0);
}

std::shared_ptr<NodeData> GraphSourceDataModel::outData(PortIndex)
{
    return _data;
}
