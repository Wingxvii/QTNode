#include "cvPlayer.h"

CVPlayer::CVPlayer(QObject *parent)
 : QThread(parent)
{
    stop = true;
}

bool CVPlayer::loadVideo(QString filename) {
    capture->open(filename.toStdString());
    if (capture->isOpened())
    {
        frameRate = capture->get(cv::CAP_PROP_FPS);

        if(frameRate == 0.0){
            frameRate = 30.0;
        }
        return true;
    }
    else
        return false;
}

bool CVPlayer::loadVideo(std::shared_ptr<VideoGraphData> videoData)
{
    LOG_JOHN() <<"Tried";

    _video = videoData->_video;
    frameRate = videoData->getFrameRate();

    if(frameRate == 0.0){
        frameRate = 30.0;
    }
    return true;
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



    LOG_JOHN() << frameRate;
    while(!stop){

        if(!_video.empty()){
            frame = _video.front();
            _video.erase(_video.begin());
        }else if (!capture->read(frame))
        {
            stop = true;
        }
        if (frame.channels()== 3){
            LOG_JOHN() << "Converting...";
            cv::cvtColor(frame, RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)(RGBframe.data),
                              RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
        }
        else
        {
            LOG_JOHN() << "Reindexing";
            img = QImage((const unsigned char*)(frame.data),
                                 frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        cv::waitKey( 1000 / frameRate );
        emit processedImage(img);

    }
}
CVPlayer::~CVPlayer()
{
    mutex.lock();
    stop = true;
    capture->release();
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
