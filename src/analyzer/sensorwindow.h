#ifndef SENSORWINDOW_H
#define SENSORWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "analyzer\filternode.h"

#include <QList>
#include <QTabWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QJSEngine>
#include <QHBoxLayout>
#include <QVBoxLayout>


namespace Ui {
class SensorWindow;
}

//this contains one node editor file
struct NodeEditorContainer{
    FilterNode* editor;
    Events* events;
    QString name;
    QString fileName;
    int index;
};

class SensorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SensorWindow(QWidget *parent = nullptr);
    void resizeEvent( QResizeEvent *e );

    //system that interacts with Shotcut
    EventContainer* getContainer();
    EventSystem sensorEvents;

private:

    Ui::SensorWindow* ui;
    QSize currentSize;
    int refreshSize;


    //menu bar items
    void createActions();
    void createMenus();

    QActionGroup *alignmentGroup;

    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* filePlaceAction;
    QAction* fileClearAction;
    QAction* fileCloseAction;

    //cardinal layouts
    QWidget* northWidget, *southWidget, *westWidget, *eastWidget, *centerWidget;
    QHBoxLayout* northLayout,* southLayout, *centerLayout;
    QVBoxLayout* westLayout, *eastLayout;
    void setupLayout();

private: //windows

    //node editor data
    QTabWidget* nodeEditorWindow;
    QList<NodeEditorContainer> nodeWindowList;
    void setupEditor();

    //console window data
    QAction* windowConsoleAction;
    QWidget* consoleWindow;
    QGridLayout* consoleWindowLayout;
    QTextEdit *consoleOutput;
    QLineEdit *consoleInput;
    QJSEngine engine;
    void setupConsole();

    //linker window data
    QWidget* linkerWindow;
    QGridLayout* linkerWindowLayout;
    QAction* windowLinkerAction;
    QTextEdit *linkerData;
    QPushButton *testbutton;
    void setupLinker();

public slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void placeSlot();
    void clearSlot();
    void closeSlot();

    void consoleSlot();
    void consoleEnterSlot();

    void linkerSlot();
};

#endif // SENSORWINDOW_H
