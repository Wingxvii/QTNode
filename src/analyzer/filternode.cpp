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
#include "nodeeditor/debuggetcorners.h"
#include "nodeeditor/convertcolor.h"
#include "nodeeditor/equalizehist.h"
#include "nodeeditor/cascadedetect.h"
#include "nodeeditor/displaydetection.h"
#include "nodeeditor/erodeimage.h"
#include "nodeeditor/dilateimage.h"
#include "nodeeditor/colorthreshold.h"
#include "nodeeditor/autotrackobject.h"
#include "nodeeditor/displaytrackedobject.h"
#include "nodeeditor/videoobjectstats.h"
#include "nodeeditor/regionofintrest.h"
#include "nodeeditor/resizevideonode.h"
#include "nodeeditor/perspectiverectifynode.h"
#include "nodeeditor/videobinningstats.h"
#include "nodeeditor/autofindfeatures.h"
#include "nodeeditor/calculateopticalflow.h"
#include "nodeeditor/trackinglist.h"
#include "nodeeditor/pointdisplay.h"
#include "nodeeditor/points2rects.h"
#include "nodeeditor/rects2points.h"
#include "nodeeditor/extractpoints.h"
#include "nodeeditor/deepimageclassification.h"
#include "nodeeditor/trainemotionmodels.h"
#include "nodeeditor/emotiondetection.h"
#include "nodeeditor/facecrop.h"
#include "nodeeditor/displayemotions.h"
#include "nodeeditor/graphemotion.h"
#include "nodeeditor/emotionkeyframer.h"

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
//
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
    final->registerModel<CalibLinkOut>("Input");
    final->registerModel<ImageLinkOut>("Input");
    final->registerModel<PointLinkOut>("Input");
    final->registerModel<PointsLinkOut>("Input");
    final->registerModel<VideoLinkOut>("Input");
    final->registerModel<DetectionLinkOut>("Input");
    final->registerModel<EmotionLinkIn>("Input");

    final->registerModel<VideoDisplay>("Output");
    final->registerModel<ImageDisplay>("Output");
    final->registerModel<CalibLinkIn>("Output");
    final->registerModel<ImageLinkIn>("Output");
    final->registerModel<PointLinkIn>("Output");
    final->registerModel<PointsLinkIn>("Output");
    final->registerModel<VideoLinkIn>("Output");
    final->registerModel<DetectionLinkIn>("Output");
    final->registerModel<PointDisplay>("Output");
    final->registerModel<EmotionLinkOut>("Output");

    final->registerModel<GetCorners>("Calibration");
    final->registerModel<Calibrate>("Calibration");
    final->registerModel<CalibInfo>("Calibration");
    final->registerModel<UnDistort>("Calibration");

    final->registerModel<convertColor>("Editing");
    final->registerModel<EqualizeHist>("Editing");
    final->registerModel<ErodeImage>("Editing");
    final->registerModel<DilateImage>("Editing");
    final->registerModel<RegionOfIntrest>("Editing");
    final->registerModel<ResizeVideoNode>("Editing");
    final->registerModel<PerspectiveRectify>("Editing");
    final->registerModel<ColorThreshold>("Editing");
    final->registerModel<FaceCrop>("Editing");


    final->registerModel<FrameIterator>("Type Conversion");
    final->registerModel<AutoFrameIterator>("Type Conversion");
    final->registerModel<Points2Rects>("Type Conversion");
    final->registerModel<Rects2Points>("Type Conversion");
    final->registerModel<ExtractPoints>("Type Conversion");

    final->registerModel<CascadeDetect>("Detectors");
    final->registerModel<AutoTrackObject>("Detectors");
    final->registerModel<AutoFindFeatures>("Detectors");
    final->registerModel<EmotionDetection>("Detectors");

    final->registerModel<DisplayDetection>("Analyzers");
    final->registerModel<DisplayTrackedObject>("Analyzers");
    final->registerModel<VideoObjectStats>("Analyzers");
    final->registerModel<VideoBinningStats>("Analyzers");
    final->registerModel<CalculateOpticalFlow>("Analyzers");
    final->registerModel<DisplayEmotions>("Analyzers");
    final->registerModel<GraphEmotion>("Analyzers");

    final->registerModel<TrackingList>("Classifer");
    final->registerModel<DeepImageClassification>("Classifer");
    final->registerModel<EmotionKeyframer>("Classifer");

    final->registerModel<DebugGetCorners>("Debug");
    final->registerModel<TrainEmotionModel>("Debug");

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
