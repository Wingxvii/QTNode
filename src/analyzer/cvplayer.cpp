#include "cvPlayer.h"

CVPlayer::CVPlayer(QObject *parent)
 : QThread(parent)
{
    stop = true;
}

bool CVPlayer::loadVideo(QString filename) {
    capture.open(filename.toStdString());
    if (capture.isOpened())
    {
        frameRate = capture.get(cv::CAP_PROP_FPS);
        return true;
    }
    else
        return false;
}
void CVPlayer::Play()
{
    if (!isRunning()) {
        if (isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}
void CVPlayer::run()
{
    while(!stop){
        if (!capture.read(frame))
        {
            stop = true;
        }
        if (frame.channels()== 3){
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        emit processedImage(img);
        cv::waitKey( 1000 / frameRate );
    }
}
CVPlayer::~CVPlayer()
{
    mutex.lock();
    stop = true;
    capture.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}
void CVPlayer::Stop()
{
    stop = true;
}

bool CVPlayer::isStopped() const{
    return this->stop;
}
