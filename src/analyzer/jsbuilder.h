#ifndef JSBUILDER_H
#define JSBUILDER_H

#include "linkmanager.h"
#include <string>

class JSBuilder: public QObject{
    Q_OBJECT

public:

    JSBuilder(){}
    ~JSBuilder(){}


public slots:
    //javascript callable functions
    Q_INVOKABLE void sendData(const QString index, int data);
    Q_INVOKABLE void sendData(const QString index, float data);
    Q_INVOKABLE void sendData(const QString index, QString data);
    Q_INVOKABLE void sendData(const QString index, bool data);

    Q_INVOKABLE void makeCalibData(const QString index,const int x, const int y, const float length);

    Q_INVOKABLE int getIntData(const QString index);
    Q_INVOKABLE float getFloatData(const QString index);
    Q_INVOKABLE QString getStringData(const QString index);
    Q_INVOKABLE bool getBoolData(const QString index);

    Q_INVOKABLE void saveData(const QString index, const int indexType);
    Q_INVOKABLE void saveImageData(const QString index);
    Q_INVOKABLE void saveVideoData(const QString index);
    Q_INVOKABLE void saveStringData(const QString index);
    Q_INVOKABLE void saveEmotionData(const QString index);
    //rest will not be able to be saved

    Q_INVOKABLE void deleteData(const QString index, const int indexType);
    Q_INVOKABLE void deleteCalibData(QString name);
    Q_INVOKABLE void deleteImageData(QString name);
    Q_INVOKABLE void deletePointData(QString name);
    Q_INVOKABLE void deletePointsData(QString name);
    Q_INVOKABLE void deleteVideoData(QString name);
    Q_INVOKABLE void deleteIntData(QString name);
    Q_INVOKABLE void deleteFloatData(QString name);
    Q_INVOKABLE void deleteStringData(QString name);
    Q_INVOKABLE void deleteBoolData(QString name);
    Q_INVOKABLE void deleteDetectionData(QString name);
    Q_INVOKABLE void deleteEmotionData(QString name);

    Q_INVOKABLE void cloneData(QString name, int type);
    Q_INVOKABLE void cloneCalibData(QString name);
    Q_INVOKABLE void cloneImageData(QString name);
    Q_INVOKABLE void clonePointData(QString name);
    Q_INVOKABLE void clonePointsData(QString name);
    Q_INVOKABLE void cloneVideoData(QString name);
    Q_INVOKABLE void cloneIntData(QString name);
    Q_INVOKABLE void cloneFloatData(QString name);
    Q_INVOKABLE void cloneStringData(QString name);
    Q_INVOKABLE void cloneBoolData(QString name);
    Q_INVOKABLE void cloneDetectionData(QString name);
    Q_INVOKABLE void cloneEmotionData(QString name);

    Q_INVOKABLE void displayData(QString name, int type);
    Q_INVOKABLE void displayCalibData(QString name);
    Q_INVOKABLE void displayImageData(QString name);
    Q_INVOKABLE void displayPointData(QString name);
    Q_INVOKABLE void displayPointsData(QString name);
    Q_INVOKABLE void displayVideoData(QString name);
    Q_INVOKABLE void displayIntData(QString name);
    Q_INVOKABLE void displayFloatData(QString name);
    Q_INVOKABLE void displayStringData(QString name);
    Q_INVOKABLE void displayBoolData(QString name);
    Q_INVOKABLE void displayDetectionData(QString name);
    Q_INVOKABLE void displayEmotionData(QString name);

    Q_INVOKABLE void loadImage(QString name);


};

#endif // JSBUILDER_H
