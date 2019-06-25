#ifndef LINKERWINDOW_H
#define LINKERWINDOW_H

#include "linkmanager.h"
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>

class LinkerWindow : public QObject{
    Q_OBJECT
public:
    LinkerWindow();
    ~LinkerWindow(){}

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* linkerWindowLayout;
    QListWidget *linkerData;
    QPushButton *testbutton;

public slots:

    void linkerUpdateSlot(int, QString);
    void linkerClearTriggered();
    void itemActivate(QListWidgetItem * item);
signals:
    void onActivate(QString, int);

};

#endif // LINKERWINDOW_H
