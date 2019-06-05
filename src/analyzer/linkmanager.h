#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include <QtCore/QList>
#include <QObject>

#include "nodeeditor/DataTypes/calibdata.h"
#include "nodeeditor/DataTypes/imagedata.h"
#include "nodeeditor/DataTypes/pointdata.h"
#include "nodeeditor/DataTypes/pointsdata.h"
#include "nodeeditor/DataTypes/videographdata.h"

//singleton that holds all linker data
class LinkManager: public QObject{
Q_OBJECT


public:
    //Singleton instance
    static LinkManager *m_instance;

    static LinkManager *instance(){
        if(!m_instance){
            m_instance = new LinkManager;
        }
        return m_instance;
    }


    void sendData(std::shared_ptr<CalibData> data, int index);
    void sendData(std::shared_ptr<ImageData> data, int index);
    void sendData(std::shared_ptr<PointData> data, int index);
    void sendData(std::shared_ptr<PointsData> data, int index);
    void sendData(std::shared_ptr<VideoGraphData> data, int index);

    std::shared_ptr<CalibData> getCalibData(int index);
    std::shared_ptr<ImageData> getImageData(int index);
    std::shared_ptr<PointData> getPointData(int index);
    std::shared_ptr<PointsData> getPointsData(int index);
    std::shared_ptr<VideoGraphData> getVideoData(int index);

    void clearAllData();

signals:
    void calibUpdated(int index);
    void imageUpdated(int index);
    void pointUpdated(int index);
    void pointsUpdated(int index);
    void videoUpdated(int index);

private:

    LinkManager();
    ~LinkManager(){}

    QList<std::shared_ptr<CalibData>> calibList;
    QList<std::shared_ptr<ImageData>> imageList;
    QList<std::shared_ptr<PointData>> pointList;
    QList<std::shared_ptr<PointsData>> pointsList;
    QList<std::shared_ptr<VideoGraphData>> videoGraphList;
};

#endif // LINKMANAGER_H
