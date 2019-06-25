#include "videowindow.h"

VideoWindow::VideoWindow()
{
    //UI

    window = new QWidget;
    layout = new QGridLayout;
    selectVideoIndex = new QLineEdit();
    selectVideoIndex->setPlaceholderText("Input Video Index");

    confirmVideoSelection = new QPushButton("Confirm Selection");
    layout->addWidget(selectVideoIndex,2,1);
    layout->addWidget(confirmVideoSelection,2,2);

    window->setLayout(layout);
    window->setVisible(false);

    connect(confirmVideoSelection, SIGNAL(clicked(bool)), this, SLOT(showVideo()));

    openAction = new QAction(tr("&Video Display"), this);
    openAction->setStatusTip("/Close Video Display Window");
    openAction->setCheckable(true);
}

void VideoWindow::showVideo()
{

}
