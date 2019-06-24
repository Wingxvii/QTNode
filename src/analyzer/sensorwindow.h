#ifndef SENSORWINDOW_H
#define SENSORWINDOW_H

#include <QMainWindow>
#include "analyzer\filternode.h"
#include "logger.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QList>

#include "savedata.h"
#include "consolewindow.h"
#include "linkerwindow.h"
#include "imagewindow.h"

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

    //file menu actions
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* filePlaceAction;
    QAction* fileClearAction;
    QAction* fileCloseAction;


    //cardinal layouts
    QWidget* northWidget, *southWidget, *centerWidget;
    QHBoxLayout* northLayout,* southLayout, *centerLayout;
    void setupLayout();

private: //windows

    //node editor data
    QTabWidget* nodeEditorWindow;
    QList<NodeEditorContainer> nodeWindowList;
    void setupEditor();

    ConsoleWindow *console;
    void setupConsole();
    LinkerWindow *linker;
    void setupLinker();
    ImageWindow *imageDisplay;
    void setUpImageDisplay();
    SaveData* saveWindow;
    void setUpDataSaver();


public slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void placeSlot();
    void clearSlot();
    void closeSlot();

    void consoleSlot();
    void linkerSlot();
    void imageDisplaySlot();

};

#endif // SENSORWINDOW_H
