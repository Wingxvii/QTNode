/*
Singleton class used to store data useful to the nodeeditor
The data stored here can be accessed freely by multiple parts of the code,
and can be interfaced with by the user.

Used by:
@linkin
@linkout
@linkerdisplay
@jsbuilder
@multithreading

*/


#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include <QtCore/QList>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QProgressBar>
#include <QLabel>
#include <iostream>
#include <QInputDialog>
#include <QMessageBox>

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
    void sendData(float data, QString name);
    void sendData(QString data, QString name);
    void sendData(bool data, QString name);

    std::shared_ptr<CalibData> getCalibData(QString name);
    std::shared_ptr<ImageData> getImageData(QString name);
    std::shared_ptr<PointData> getPointData(QString name);
    std::shared_ptr<PointsData> getPointsData(QString name);
    std::shared_ptr<VideoGraphData> getVideoData(QString name);
    int getIntData(QString name);
    float getFloatData(QString name);
    QString getStringData(QString name);
    bool getBoolData(QString name);

    void saveData(QString name, int type);
    void saveImageData(QString name);
    void saveVideoData(QString name);
    void saveStringData(QString name);

    void deleteData(QString name, int type);
    void deleteCalibData(QString name);
    void deleteImageData(QString name);
    void deletePointData(QString name);
    void deletePointsData(QString name);
    void deleteVideoData(QString name);
    void deleteIntData(QString name);
    void deleteFloatData(QString name);
    void deleteStringData(QString name);
    void deleteBoolData(QString name);

    void cloneData(QString name, int type);
    void cloneCalibData(QString name);
    void cloneImageData(QString name);
    void clonePointData(QString name);
    void clonePointsData(QString name);
    void cloneVideoData(QString name);
    void cloneIntData(QString name);
    void cloneFloatData(QString name);
    void cloneStringData(QString name);
    void cloneBoolData(QString name);

    void displayData(QString name, int type);
    void displayCalibData(QString name);
    void displayImageData(QString name);
    void displayPointData(QString name);
    void displayPointsData(QString name);
    void displayVideoData(QString name);
    void displayIntData(QString name);
    void displayFloatData(QString name);
    void displayStringData(QString name);
    void displayBoolData(QString name);

    //clears public data
    void clearAllData();

    //clears private data
    void privateClear();

    //returns a string vector for data display
    std::map<QString, int> getAllData(int x = -1);



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
    std::map<QString, int> intList;
    std::map<QString, float> floatList;
    std::map<QString, QString> stringList;
    std::map<QString, bool> boolList;


    //secondary data dump for internal use
    std::map<QString, std::shared_ptr<CalibData>> calibListPrivate;
    std::map<QString, std::shared_ptr<ImageData>> imageListPrivate;
    std::map<QString, std::shared_ptr<PointData>> pointListPrivate;
    std::map<QString, std::shared_ptr<PointsData>> pointsListPrivate;
    std::map<QString, std::shared_ptr<VideoGraphData>> videoGraphListPrivate;
    std::map<QString, int> intListPrivate;
    std::map<QString, float> floatListPrivate;
    std::map<QString, QString> stringListPrivate;
    std::map<QString, bool> boolListPrivate;


//multithreading
public:
    void multiThreadVideoSave();
    QFuture<void> functVidSave;
};

#endif // LINKMANAGER_H
