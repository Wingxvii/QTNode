#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include "linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "cvPlayer.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>

class VideoWindow : public QObject{
    Q_OBJECT

public:
    VideoWindow();
    ~VideoWindow(){}

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* layout;
    QAction* displayAction;
    CVPlayer* myPlayer;
    QLabel* displayLabel;
    QPushButton* playButton;
    QPushButton* loadButton;
    QPushButton* clearButton;

    QVector<QPixmap> videoToPlay;
    QPixmap currFrame;
    double videoFrameRate;
    bool videoIsReady;

    QFuture<void> funct;
    QFutureWatcher<void> functWatcher;


public slots:
    void updatePlayerUI(QImage img);
    void videoReady(double frameRate);
    void onLoad();
    void onPlay();
    void onVideoClear();

    void multiThreadedProcess();
    void multiThreadedFinished();
    void playLocal();

signals:
    void imageUpdate();

};

#endif // VIDEOWINDOW_H
