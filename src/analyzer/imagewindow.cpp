#include "imagewindow.h"

ImageWindow::ImageWindow()
{
    //UI
    window = new QWidget;
    imageWindowLayout = new QGridLayout;
    selectImageIndex = new QLineEdit();
    selectImageIndex->setPlaceholderText("Input Image Index");
    imageShow = new QLabel;
    confirmImageSelection = new QPushButton("Confirm Selection");

    imageWindowLayout->addWidget(imageShow,1,1,2,1);
    imageWindowLayout->addWidget(selectImageIndex,2,1);
    imageWindowLayout->addWidget(confirmImageSelection,2,2);

    window->setLayout(imageWindowLayout);
    window->setVisible(false);

    connect(confirmImageSelection, SIGNAL(clicked(bool)), this, SLOT(showImage()));

    openAction = new QAction(tr("&Image Display"), this);
    openAction->setStatusTip("Open/Close Image Display Window");
    openAction->setCheckable(true);

}

void ImageWindow::showImage()
{
    cv::Mat img;

    if(LinkManager::instance()->getImageData(selectImageIndex->text())){
        img = LinkManager::instance()->getImageData(selectImageIndex->text())->_image;
        cv::cvtColor(img,img,cv::COLOR_RGB2BGR);
        imageShow->setPixmap(QPixmap::fromImage((QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888))));

    }else{
        LOG_JOHN() << "No Data Found";
        QPixmap img("C:/projects/Shotcut/src/shotcut/IMAGES/nothing.png");
        imageShow->setPixmap(img);
    }

}
