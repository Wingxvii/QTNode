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


    void sendData(std::shared_ptr<CalibData> data, QString name);
    void sendData(std::shared_ptr<ImageData> data, QString name);
    void sendData(std::shared_ptr<PointData> data, QString name);
    void sendData(std::shared_ptr<PointsData> data, QString name);
    void sendData(std::shared_ptr<VideoGraphData> data, QString name);
    void sendData(int data, QString name);
    void sendData(QString data, QString name);

    std::shared_ptr<CalibData> getCalibData(QString name);
    std::shared_ptr<ImageData> getImageData(QString name);
    std::shared_ptr<PointData> getPointData(QString name);
    std::shared_ptr<PointsData> getPointsData(QString name);
    std::shared_ptr<VideoGraphData> getVideoData(QString name);
    int getIntData(QString name);
    QString getNameData(QString name);

    void clearAllData();
    void privateClear();
    std::vector<QString> getAllData();

signals:
    void updated(int dataIndex, QString name);

private:

    LinkManager();
    ~LinkManager(){}

    std::map<QString, std::shared_ptr<CalibData>> calibList;
    std::map<QString, std::shared_ptr<ImageData>> imageList;
    std::map<QString, std::shared_ptr<PointData>> pointList;
    std::map<QString, std::shared_ptr<PointsData>> pointsList;
    std::map<QString, std::shared_ptr<VideoGraphData>> videoGraphList;

    //secondary data dump for internal use
    std::map<QString, std::shared_ptr<CalibData>> calibListPrivate;
    std::map<QString, std::shared_ptr<ImageData>> imageListPrivate;
    std::map<QString, std::shared_ptr<PointData>> pointListPrivate;
    std::map<QString, std::shared_ptr<PointsData>> pointsListPrivate;
    std::map<QString, std::shared_ptr<VideoGraphData>> videoGraphListPrivate;
    std::map<QString, int> intListPrivate;
    std::map<QString, QString> stringListPrivate;


};

#endif // LINKMANAGER_H
