#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include <QtCore/QList>

#include "nodeeditor/DataTypes/calibdata.h"
#include "nodeeditor/DataTypes/imagedata.h"
#include "nodeeditor/DataTypes/pointdata.h"
#include "nodeeditor/DataTypes/pointsdata.h"
#include "nodeeditor/DataTypes/videographdata.h"


//singleton that holds all linker data
class LinkManager{
public:
    static void sendData(CalibData data, int index);
    static void sendData(ImageData data, int index);
    static void sendData(PointData data, int index);
    static void sendData(PointsData data, int index);
    static void sendData(VideoGraphData data, int index);

    static CalibData getCalibData(int index);
    static ImageData getImageData(int index);
    static PointData getPointData(int index);
    static PointsData getPointsData(int index);
    static VideoGraphData getVideoData(int index);

    static void clearAllData();

public: signals:
    void calibUpdated();
    void imageUpdated();
    void pointUpdated();
    void pointsUpdated();
    void videoUpdated();

private:

    static QList<CalibData> calibList;
    static QList<ImageData> imageList;
    static QList<PointData> pointList;
    static QList<PointsData> pointsList;
    static QList<VideoGraphData> videoGraphList;
};

#endif // LINKMANAGER_H
