#ifndef FILTERNODE_H
#define FILTERNODE_H
#include "QDialog"
#include "QHBoxLayout"

#include <nodes\NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/ConnectionStyle>
#include <nodes/TypeConverter>
#include <nodes/DataModelRegistry>
#include <nodes/NodeDataModel>

using QtNodes::NodeDataModel;
using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;
using QtNodes::ConnectionStyle;
using QtNodes::TypeConverter;
using QtNodes::TypeConverterId;

#include "nodeeditor/outputdisplaymodel.h"

class FilterNode : public QWidget
{
    Q_OBJECT
public:
    FilterNode(Events* events, QWidget* parent = 0);
    ~FilterNode();

    QHBoxLayout *layout;
    std::unique_ptr<NodeDataModel> type;
    OutputDisplayModel* output;
    FlowScene* scene;

public slots:
    void onDeletedNode(QtNodes::Node &n);
    void onCreatedNode(QtNodes::Node &n);
    void onCreateEvents(std::vector<float>);
    void onDeleteEvents(std::vector<float>);

private:
    std::shared_ptr<DataModelRegistry> outputSensor;
    std::shared_ptr<DataModelRegistry> final;

    Events* events;
    int currentIndex;
    int index;

};


#endif // FILTERNODE_H
