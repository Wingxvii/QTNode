#include "cvPlayer.h"

CVPlayer::CVPlayer(QObject *parent)
 : QThread(parent)
{
    stop = true;
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

}

bool CVPlayer::loadVideo(QString filename) {
    VideoCapture capture;
    capture.open(filename.toStdString());


    if (capture.isOpened())
    {
        frameRate = capture.get(cv::CAP_PROP_FPS);
        maxFrame = capture.get(cv::CAP_PROP_FRAME_COUNT);
        if(frameRate == 0.0){
            frameRate = 30.0;
        }

        funct = QtConcurrent::run(this, &CVPlayer::multiThreadedProcess, capture);
        functWatcher.setFuture(funct);


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
    maxFrame = videoData->_video.size();

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
        start(HighPriority);
    }
}
void CVPlayer::run()
{

    LOG_JOHN() << frameRate;
    while(!stop){

        if(currFrame < maxFrame){
            frame = _video.at(currFrame);
            currFrame++;
        }else{
            stop = true;
            emit endReached();
        }
        emit framePosition(currFrame);

        if (frame.channels()== 3){
            //LOG_JOHN() << "Converting..." << _video.size();
            cv::cvtColor(frame, RGBframe, cv::COLOR_RGB2BGR);
            //LOG_JOHN() << "Reformatting..." << _video.size();
            img = QImage((const unsigned char*)(RGBframe.data), RGBframe.cols,RGBframe.rows,QImage::Format_RGB888);
            //LOG_JOHN() << "Done!";
        }
        else
        {
            //LOG_JOHN() << "Reindexing...";
            img = QImage((const unsigned char*)(frame.data), frame.cols,frame.rows,QImage::Format_Indexed8);
            //LOG_JOHN() << "Done!";
        }
        QThread::msleep( 1000 / (frameRate*frameMultiplier) );
        emit processedImage(img);
    }
}

void CVPlayer::multiThreadedProcess(VideoCapture capture)
{
    cv::Mat temp;
    capture >> temp;
    int start = 1;

    while (!temp.empty())
    {
        _video.push_back(temp.clone());
        capture >> temp;
        start++;
    }

    double fps = capture.get(cv::CAP_PROP_FPS);

}

void CVPlayer::multiThreadedFinished()
{
    emit doneLoading();
}
CVPlayer::~CVPlayer()
{
    mutex.lock();
    stop = true;
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
