#include "videowindow.h"

VideoWindow::VideoWindow()
{
    //UI

    window = new QWidget;
    layout = new QGridLayout;
    myPlayer = new CVPlayer();
    displayLabel = new QLabel();
    playButton = new QPushButton("Play");
    loadButton = new QPushButton("Load from File...");
    infoLabel = new QLabel("Video Info");

    selectVideoIndex = new QLineEdit();
    confirmVideoSelection = new QPushButton("Load from Cache");

    layout->addWidget(infoLabel,1,1);
    layout->addWidget(displayLabel,2,1,1,3);
    layout->addWidget(selectVideoIndex,3,1);
    layout->addWidget(confirmVideoSelection,3,2);
    layout->addWidget(loadButton,3,3);
    layout->addWidget(playButton,4,2);

    window->setLayout(layout);
    window->setVisible(false);

    connect(myPlayer, SIGNAL(processedImage(QImage)), this, SLOT(updatePlayerUI(QImage)));
    connect(playButton, SIGNAL(clicked(bool)), this, SLOT(onPlay()));
    connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(onLoad()));
    connect(confirmVideoSelection, SIGNAL(clicked(bool)), this, SLOT(getVideo()));
    connect(myPlayer, SIGNAL(doneLoading()), this, SLOT(finishedProcessing()));
    connect(myPlayer, SIGNAL(endReached()), this, SLOT(handleReplay()));

    openAction = new QAction(tr("&Video Display"), this);
    openAction->setStatusTip("/Close Video Display Window");
    openAction->setCheckable(true);


}
void VideoWindow::updatePlayerUI(QImage img)
{
    //LOG_JOHN() << "Signal Recieved";
    if (!img.isNull())
    {
        displayLabel->setAlignment(Qt::AlignCenter);
        displayLabel->setPixmap(QPixmap::fromImage(img).scaled(displayLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        LOG_JOHN() << "Signal Displayed";
    }
}
void VideoWindow::onLoad()
{
    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR,
                                          tr("Open Video"), ".",
                                          tr("Video Files (*.avi *.mpg *.mp4)"));
    infoLabel->setText("Video Loaded from Memory");
    if (!filename.isEmpty()){
        if (!myPlayer->loadVideo(filename.toLatin1().data()))
        {
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }else{
            playButton->setText("Processing...");
            playButton->setEnabled(false);

        }

    }
}
void VideoWindow::onPlay()
{
    if(playButton->text() == "Replay"){
        myPlayer->currFrame = 1;
        myPlayer->Play();
        playButton->setText(tr("Pause"));
    }
    else if (myPlayer->isStopped())
    {
        myPlayer->Play();
        playButton->setText(tr("Pause"));

    }
    else
    {
        myPlayer->Stop();
        playButton->setText(tr("Play"));
    }
}

void VideoWindow::getVideo()
{
    if(LinkManager::instance()->getVideoData(selectVideoIndex->text())){
        myPlayer->loadVideo(LinkManager::instance()->getVideoData(selectVideoIndex->text()));
        infoLabel->setText("Video Loaded from cache");
    }else{
        infoLabel->setText("Video Not Found");
    }
}

void VideoWindow::finishedProcessing()
{
    playButton->setText("Play");
    playButton->setEnabled(true);

}

void VideoWindow::handleReplay()
{
    playButton->setText("Replay");

}
