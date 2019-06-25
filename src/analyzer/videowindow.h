#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include "linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>

class VideoWindow : public QObject{
    Q_OBJECT

public:
    VideoWindow();
    ~VideoWindow(){};

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* layout;
    QAction* displayAction;
    QLineEdit* selectVideoIndex;
    QPushButton* confirmVideoSelection;


public slots:
    void showVideo();
};

#endif // VIDEOWINDOW_H
