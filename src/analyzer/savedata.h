#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

class SaveData : public QObject{
    Q_OBJECT

public:
    SaveData();
    ~SaveData(){}

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* layout;
    QPushButton* deleteButton;
    QPushButton* saveButton;
    QPushButton* cloneButton;
    QPushButton* displayButton;
    QLabel* dataIndexDisplay;

    QLineEdit* nameIndexInput;
    QComboBox* typeIndexInput;

    int typeIndex;
    QString nameIndex;

private:
    void setUpTypes();

public slots:
    void openSaveWindow();
    void openSaveWindow(QString, int);
    void onDelete();
    void onSave();
    void onClone();
    void onDisplay();

    void updateLabel();

};

#endif // SAVEDATA_H
