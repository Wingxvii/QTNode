#include "videowindow.h"

VideoWindow::VideoWindow()
{
    //UI

    window = new QWidget;
    layout = new QGridLayout;
    myPlayer = new CVPlayer();
    displayLabel = new QLabel();
    playButton = new QPushButton("Waiting for Source...");
    playButton->setEnabled(false);
    loadButton = new QPushButton("Load");
    clearButton = new QPushButton("Clear");

    layout->addWidget(displayLabel,1,1,1,3);
    layout->addWidget(playButton,2,1);
    layout->addWidget(loadButton,2,2);
    layout->addWidget(clearButton,2,3);

    window->setLayout(layout);
    window->setVisible(false);

    connect(myPlayer, SIGNAL(processedImage(QImage)), this, SLOT(updatePlayerUI(QImage)));
    connect(myPlayer, SIGNAL(doneProcessing(double)), this, SLOT(videoReady(double)));
    connect(this, SIGNAL(imageUpdate()), this, SLOT(playLocal()));

    connect(playButton, SIGNAL(clicked(bool)), this, SLOT(onPlay()));
    connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(onLoad()));
    connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(onVideoClear()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    openAction = new QAction(tr("&Video Display"), this);
    openAction->setStatusTip("/Close Video Display Window");
    openAction->setCheckable(true);


}
void VideoWindow::updatePlayerUI(QImage img)
{
    if(!videoIsReady){
    LOG_JOHN() << "Process Recieved";
    if (!img.isNull())
    {
        //displayLabel->setAlignment(Qt::AlignCenter);
        videoToPlay.append(QPixmap::fromImage(img).scaled(displayLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
    }
    }else{
        LOG_JOHN() << "Frame Deleted";
    }
}

void VideoWindow::videoReady(double frameRate)
{
    LOG_JOHN() << "Processing Done";
    videoFrameRate =frameRate;
    videoIsReady = true;
    playButton->setEnabled(true);
    playButton->setText(tr("Play"));
}

void VideoWindow::onLoad()
{
    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR,
                                          tr("Open Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4)"));
    if (!filename.isEmpty()){
        if (!myPlayer->loadVideo(filename.toLatin1().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }else{
            videoIsReady = false;
            playButton->setText(tr("Processing"));
            playButton->setEnabled(false);
        }
    }
}
void VideoWindow::onPlay()
{
    //handles start pausing and resuming
    if(videoIsReady && !funct.isRunning()){
        funct = QtConcurrent::run(this, &VideoWindow::multiThreadedProcess);
        functWatcher.setFuture(funct);
        playButton->setText("Pause");
    }else if(funct.isRunning() && funct.isPaused()){
        funct.resume();
        playButton->setText("Pause");
    }else if(funct.isRunning()){
        funct.pause();
        playButton->setText("Resume");
    }
}

void VideoWindow::onVideoClear()
{
    if(funct.isRunning()){
        funct.cancel();
    }
    videoIsReady = false;
    videoToPlay.clear();
    displayLabel->clear();
}

void VideoWindow::multiThreadedProcess()
{
    for(QPixmap image : videoToPlay){
        currFrame = image;
        emit imageUpdate();
        QThread::msleep(1000/videoFrameRate);
    }
}

void VideoWindow::multiThreadedFinished()
{
    LOG_JOHN() << "Video Finished Playing";
    playButton->setText("Play Again?");
}

void VideoWindow::playLocal()
{
    displayLabel->setAlignment(Qt::AlignCenter);
    displayLabel->setPixmap(currFrame);
}
