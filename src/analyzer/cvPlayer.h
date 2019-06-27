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
#include <QtConcurrent/QtConcurrent>

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
    Mat RGBframe;
    QImage img;

public:
    std::vector<cv::Mat> _video;
    double frameMultiplier = 1;

    unsigned int currFrame = 1;
    unsigned int maxFrame;

 signals:
 //Signal to output frame to be displayed
      void processedImage(const QImage &image);
      void doneLoading();
      void endReached();
      void framePosition(int);
 protected:
     void run();

public:
     QFuture<void> funct;
     QFutureWatcher<void> functWatcher;

public slots:
     void multiThreadedProcess(cv::VideoCapture capture);
     void multiThreadedFinished();


 public:
    //Constructor
    CVPlayer(QObject *parent = 0);
    //Destructor
    ~CVPlayer();
    //Load a video from memory
    bool loadVideo(QString filename);
    //Load a video from memory
    bool loadVideo(std::shared_ptr<VideoGraphData> videoData);


    //Play the video
    void Play();
    //Stop the video
    void Stop();
    //check if the player has been stopped
    bool isStopped() const;
};
#endif // VIDEOPLAYER_H
