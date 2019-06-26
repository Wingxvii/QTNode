#ifndef CVPLAYER_H
#define CVPLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "nodeeditor/DataTypes/videographdata.h"

using namespace cv;
class CVPlayer : public QThread
{
    Q_OBJECT
 private:
    bool stop;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    double frameRate;
    VideoCapture capture;
    Mat RGBframe;
    QImage img;
 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
 protected:
     void run();
 public:
    //Constructor
    CVPlayer(QObject *parent = 0);
    //Destructor
    ~CVPlayer();
    //Load a video from memory
    bool loadVideo(QString filename);
    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
};
#endif // VIDEOPLAYER_H
