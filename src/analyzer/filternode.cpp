#include "filternode.h"

#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/ConnectionStyle>
#include <nodes/TypeConverter>
#include <nodes/Node>

#include "nodeeditor/videooutputmodel.h"
#include "nodeeditor/outputdisplaymodel.h"


//#include "nodeeditor/graphsourcedatamodel.h"
//#include "nodeeditor/graphdisplaydatamodel.h"
//#include "nodeeditor/minmodel.h"
//#include "nodeeditor/maxmodel.h"
//#include "nodeeditor/additive.h"
//#include "nodeeditor/backgroundsubtraction.h"
//#include "nodeeditor/debuggetcorners.h"
//#include "nodeeditor/checkerboardpointsbuffer.h"
//#include "nodeeditor/tohsv.h"
//#include "nodeeditor/videothreshold.h"
//#include "nodeeditor/erodeimage.h"
//#include "nodeeditor/dilateimage.h"
//#include "nodeeditor/blobfilternode.h"


#include "nodeeditor/videosourcedatamodel.h"
#include "nodeeditor/frameiterator.h"
#include "nodeeditor/autoframeiterator.h"
#include "nodeeditor/calibrate.h"
#include "nodeeditor/getcorners.h"
#include "nodeeditor/calibinfo.h"
#include "nodeeditor/undistort.h"
#include "nodeeditor/videodisplay.h"
#include "nodeeditor/imagedisplay.h"
#include "nodeeditor/linkin.h"
#include "nodeeditor/linkout.h"


FilterNode::FilterNode(Events* events, QWidget* parent) :
    QWidget(parent)
{
    this->events = events;

    //setup layout
    layout = new QHBoxLayout();
    setLayout(layout);

    //create the output display sensor
    outputSensor = std::make_shared<DataModelRegistry>();
    outputSensor->registerModel<OutputDisplayModel>("Output");

    //create the qt element of the editor
    scene = new FlowScene(outputSensor);

    final = std::make_shared<DataModelRegistry>();
    //Register the different nodes

    //Debug
//    final->registerModel<GraphSourceDataModel>("zLegacy"); //Sources
//    final->registerModel<GraphDisplayDataModel>("zLegacy"); //Displays
//    final->registerModel<OutputDisplayModel>("zLegacy");//output
//    final->registerModel<VideoOutputModel>("zLegacy");//output
//    final->registerModel<DebugGetCorners>("zLegacy");
//    final->registerModel<CheckerboardPointsBuffer>("Calibration");
//    final->registerModel<VideoThreshold>("Tracking");
//    final->registerModel<ErodeImage>("Tracking");
//    final->registerModel<DilateImage>("Tracking");
//    final->registerModel<ToHSV>("Operations");
//    final->registerModel<MinModel>("Operations");
//    final->registerModel<MaxModel>("Operations");
//    final->registerModel<Additive>("Operations");
//    final->registerModel<BlobFilterNode>("Operations");
//    final->registerModel<BackgroundSubtraction>("Operations");

    //Working Nodes
    final->registerModel<VideoSourceDataModel>("Input");
    final->registerModel<FrameIterator>("Input");
    final->registerModel<AutoFrameIterator>("Input");

    final->registerModel<VideoDisplay>("Output");
    final->registerModel<ImageDisplay>("Output");

    final->registerModel<GetCorners>("Calibration");
    final->registerModel<Calibrate>("Calibration");
    final->registerModel<CalibInfo>("Calibration");
    final->registerModel<UnDistort>("Calibration");

    final->registerModel<CalibLinkIn>("Link");
    final->registerModel<ImageLinkIn>("Link");
    final->registerModel<PointLinkIn>("Link");
    final->registerModel<PointsLinkIn>("Link");
    final->registerModel<VideoLinkIn>("Link");

    final->registerModel<CalibLinkOut>("Link");
    final->registerModel<ImageLinkOut>("Link");
    final->registerModel<PointLinkOut>("Link");
    final->registerModel<PointsLinkOut>("Link");
    final->registerModel<VideoLinkOut>("Link");

    scene->setRegistry(final);
    layout->addWidget(new FlowView(scene));


    connect(scene, &FlowScene::nodeCreated, this, &FilterNode::onCreatedNode);
    connect(scene, &FlowScene::nodeDeleted, this, &FilterNode::onDeletedNode);
}


void FilterNode::onDeletedNode(QtNodes::Node &n)
{
    const OutputDisplayModel* graphOutput = dynamic_cast<const OutputDisplayModel*>(n.nodeDataModel());
    if (graphOutput)
    {
        //if we ever need to do some cleanup
    }
    const VideoOutputModel* videoOutput = dynamic_cast<const VideoOutputModel*>(n.nodeDataModel());
    if (videoOutput)
    {
        //if we ever need to do some cleanup
    }
}

void FilterNode::onCreatedNode(QtNodes::Node &n)
{

    const OutputDisplayModel* graphOutput = dynamic_cast<const OutputDisplayModel*>(n.nodeDataModel());
    if (graphOutput != NULL)
    {
        connect(graphOutput, SIGNAL(sendCreateEvents(std::vector<float>)), this, SLOT(onCreateEvents(std::vector<float>)));
        connect(graphOutput, SIGNAL(sendDeleteEvents(std::vector<float>)), this, SLOT(onDeleteEvents(std::vector<float>)));
    }

    const VideoOutputModel* videoOutput = dynamic_cast<const VideoOutputModel*>(n.nodeDataModel());
    if (videoOutput != NULL)
    {
        connect(videoOutput, SIGNAL(sendCreateEvents(std::vector<float>)), this, SLOT(onCreateEvents(std::vector<float>)));
        connect(videoOutput, SIGNAL(sendDeleteEvents(std::vector<float>)), this, SLOT(onDeleteEvents(std::vector<float>)));
    }
}

void FilterNode::onCreateEvents(std::vector<float> data)
{
    for (auto it : data)
    {
        LOG_CAMPBELL() << "Data: " + QString::number(it);
        events->addEvent(it);
    }
}

void FilterNode::onDeleteEvents(std::vector<float> data)
{
    for (auto it : data)
    {
        LOG_CAMPBELL() << "Data: " + QString::number(it);
        events->removeEvent(it);
    }
}

FilterNode::~FilterNode()
{

}
