#ifndef JSBUILDER_H
#define JSBUILDER_H

#include "linkmanager.h"

class JSBuilder: public QObject{
    Q_OBJECT

public:
    //Singleton instance
    static JSBuilder *m_instance;

    static JSBuilder *instance(){
        if(!m_instance){
            m_instance = new JSBuilder;
        }
        return m_instance;
    }

private:
    JSBuilder(){}
    ~JSBuilder(){}


public slots:
    //javascript callable functions
    Q_INVOKABLE void sendData(const QString dataType, const QString index, int data);
    Q_INVOKABLE void sendData(const QString dataType, const QString index, float data);
    Q_INVOKABLE void sendData(const QString dataType, const QString index, QString data);
    Q_INVOKABLE void sendData(const QString dataType, const QString index, bool data);

    Q_INVOKABLE QObject* recieveData(const QString dataType, const QString index);

};

#endif // JSBUILDER_H
