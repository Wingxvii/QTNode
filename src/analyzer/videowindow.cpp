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
    slider = new QSlider(Qt::Horizontal);
    playbackSpeed = new QComboBox();

    selectVideoIndex = new QLineEdit();
    confirmVideoSelection = new QPushButton("Load from Cache");

    layout->addWidget(infoLabel,2,1,1,3);
    layout->addWidget(displayLabel,3,1,1,3);
    layout->addWidget(selectVideoIndex,1,1);
    layout->addWidget(confirmVideoSelection,1,2);
    layout->addWidget(loadButton,1,3);
    layout->addWidget(slider,4,1);
    layout->addWidget(playButton,4,2);
    layout->addWidget(playbackSpeed,4,3);

    playbackSpeed->addItem("0.25");
    playbackSpeed->addItem("0.5");
    playbackSpeed->addItem("1");
    playbackSpeed->addItem("1.5");
    playbackSpeed->addItem("2");
    playbackSpeed->addItem("5");
    playbackSpeed->setCurrentIndex(2);


    window->setLayout(layout);
    window->setVisible(false);

    connect(myPlayer, SIGNAL(processedImage(QImage)), this, SLOT(updatePlayerUI(QImage)));
    connect(playButton, SIGNAL(clicked(bool)), this, SLOT(onPlay()));
    connect(loadButton, SIGNAL(clicked(bool)), this, SLOT(onLoad()));
    connect(confirmVideoSelection, SIGNAL(clicked(bool)), this, SLOT(getVideo()));
    connect(myPlayer, SIGNAL(doneLoading()), this, SLOT(finishedProcessing()));
    connect(myPlayer, SIGNAL(endReached()), this, SLOT(handleReplay()));

    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderOut()));
    connect(myPlayer, SIGNAL(framePosition(int)), this, SLOT(sliderIn(int)));
    connect(playbackSpeed, SIGNAL(activated(int)), this, SLOT(updatePlayBack(int)));
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
        //LOG_JOHN() << "Signal Displayed";
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
        slider->setMaximum(myPlayer->maxFrame);
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

void VideoWindow::sliderIn(int index)
{
    //LOG_JOHN() << "Slider Updated In: " << index;

    if(index < slider->maximum()){
        slider->setSliderPosition(index);
    }
}

void VideoWindow::sliderOut()
{
    //LOG_JOHN() << "Slider Updated Out: " << slider->value();

    if(playButton->text() == "Replay" && slider->value() < slider->maximum()){
        playButton->setText("Play");
    }

    myPlayer->currFrame = slider->value();
}

void VideoWindow::updatePlayBack(int index)
{
    switch(index){
    case 0:
        myPlayer->frameMultiplier = 0.25;
        break;
    case 1:
        myPlayer->frameMultiplier = 0.5;
        break;
    case 2:
        myPlayer->frameMultiplier = 1.0;
        break;
    case 3:
        myPlayer->frameMultiplier = 1.5;
        break;
    case 4:
        myPlayer->frameMultiplier = 2;
        break;
    case 5:
        myPlayer->frameMultiplier = 5;
        break;
    }
}
