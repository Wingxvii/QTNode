#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "linkmanager.h"
#include "Logger.h"

class SaveData : public QObject{
    Q_OBJECT

public:
    SaveData(){}
    ~SaveData(){}

public slots:
    void openSaveWindow();

};

#endif // SAVEDATA_H
