#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include "linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "cvPlayer.h"
#include <QSlider>
#include <QComboBox>

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
    QSlider* slider;
    QComboBox* playbackSpeed;

    QLineEdit* selectVideoIndex;
    QPushButton* confirmVideoSelection;
    QLabel* infoLabel;

public slots:
    void updatePlayerUI(QImage img);
    void onLoad();
    void onPlay();
    void getVideo();
    void finishedProcessing();
    void handleReplay();

    void sliderIn(int index);
    void sliderOut();
    void updatePlayBack(int index);

};

#endif // VIDEOWINDOW_H
