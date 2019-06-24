#ifndef CONSOLEWINDOW_H
#define CONSOLEWINDOW_H
#include "jsbuilder.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QJSEngine>
#include <QGridLayout>

class ConsoleWindow : public QObject{
    Q_OBJECT

public:
    ConsoleWindow();
    ~ConsoleWindow(){};

    //main widget
    QWidget* window;
    //open action
    QAction* openAction;

private:
    QGridLayout* consoleWindowLayout;
    QTextEdit *consoleOutput;
    QLineEdit *consoleInput;
    QJSEngine engine;
    JSBuilder* builder;

public slots:
    void consoleEnterSlot();


};

#endif // CONSOLEWINDOW_H
