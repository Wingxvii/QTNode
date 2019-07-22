#ifndef ADDFEATURE_H
#define ADDFEATURE_H

#include "analyzer/linkmanager.h"
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QRegExpValidator>

class AddFeature : public QObject
{
    Q_OBJECT
public:
    AddFeature();
    ~AddFeature(){}

    //main window
    QWidget *window;

private:
    QGridLayout *layout;
    QPushButton *enterButton;
    QPushButton *cancelButton;
    QLabel *inputStart;
    QLabel *inputMiddle;
    QLabel *inputEnd;
    QLineEdit *xInput;
    QLineEdit *yInput;

    QLabel *nameInputLabel;
    QLineEdit *nameInput;

    int x = -1;
    int y = -1;
    QString name = "";

    QRegExpValidator* intPos;

public slots:
    void openWindow();
    void onEnter();
    void onExit();

signals:
    void sendPoint(int x,int y, QString name);

};

#endif // ADDFEATURE_H
