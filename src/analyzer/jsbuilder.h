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

    Q_INVOKABLE int getIntData(const QString index);
    Q_INVOKABLE float getFloatData(const QString index);
    Q_INVOKABLE QString getStringData(const QString index);
    Q_INVOKABLE bool getBoolData(const QString index);

    Q_INVOKABLE void saveData(const QString index, const int indexType);
    Q_INVOKABLE void saveImageData(const QString index);
    Q_INVOKABLE void saveVideoData(const QString index);
    Q_INVOKABLE void saveStringData(const QString index);


};

#endif // JSBUILDER_H
