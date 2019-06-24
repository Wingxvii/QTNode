#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H


#include "linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>

class ImageWindow : QObject{
    Q_OBJECT
public:
    ImageWindow();
    ~ImageWindow(){};

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* imageWindowLayout;
    QAction* imageDisplayAction;
    QLineEdit* selectImageIndex;
    QLabel* imageShow;
    QPushButton* confirmImageSelection;

public slots:
    void showImage();


};


#endif // IMAGEWINDOW_H
