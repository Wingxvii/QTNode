#include "videooutputmodel.h"

VideoOutputModel::VideoOutputModel()
{
    layout = new QGridLayout;
    window = new QWidget();


    button = new QPushButton("Create Events");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(createEvents()));
    layout->addWidget(button);

    window->setLayout(layout);

}


unsigned int VideoOutputModel::nPorts(PortType portType) const
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

NodeDataType VideoOutputModel::dataType(PortType portType, PortIndex portIndex) const
{
    return VideoGraphData().type();
}

std::shared_ptr<NodeData> VideoOutputModel::outData(QtNodes::PortIndex port)
{
    std::shared_ptr<NodeData> ptr;
    return ptr;
}

void VideoOutputModel::setInData(std::shared_ptr<NodeData> data, int)
{
    _data = std::dynamic_pointer_cast<VideoGraphData>(data);

    if (_data)
    {
        LOG_CAMPBELL() << "Data exists";
        modelValidationState = NodeValidationState::Valid;
        modelValidationError = QString();
    }
    else
    {
        emit sendDeleteEvents(events);
        events.clear();

        modelValidationState = NodeValidationState::Warning;
        modelValidationError = QStringLiteral("Missing or incorrect inputs");
        LOG_CAMPBELL() << "data doesnt exist";
    }
}

void VideoOutputModel::createEvents()
{
    if (_data == NULL)
        return;

    emit sendDeleteEvents(events);
    events.clear();

    float frameRate = _data->getFrameRate();
    auto frames = _data->data();
    for (int it = 0; it < frames.size(); it++)
    {
        int id = it;
        LOG_CAMPBELL() << "frame: " + QString::number(id);
        events.push_back(it / frameRate);
    }
    emit sendCreateEvents(events);

}

NodeValidationState VideoOutputModel::validationState() const
{
    return modelValidationState;
}

QString VideoOutputModel::validationMessage() const
{
    return modelValidationError;
}
