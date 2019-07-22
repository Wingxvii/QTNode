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
    int row = 0;
    QString name = "";

    bool edit = false;
    QRegExpValidator* intPos;

public slots:
    void openWindow();
    void onEnter();
    void onExit();
    void onEdit(int row, int x, int y, QString name);

signals:
    void sendPoint(int x,int y, QString name);
    void sendEdit(int row, int x,int y, QString name);

};

#endif // ADDFEATURE_H
