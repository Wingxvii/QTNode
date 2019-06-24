#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "linkmanager.h"

class SaveData : public QObject{
    Q_OBJECT

public:
    SaveData();
    ~SaveData(){}

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;


public slots:
    void openSaveWindow();

};

#endif // SAVEDATA_H
