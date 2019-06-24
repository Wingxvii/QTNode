#ifndef SENSORWINDOW_H
#define SENSORWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "analyzer\filternode.h"
#include "logger.h"

#include <QList>
#include <QTabWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>

#include <QJSEngine>
#include <QJSValue>

#include <QQmlEngine>
#include <QQmlContext>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "jsbuilder.h"
#include "savedata.h"

#include "consolewindow.h"
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

    QAction* saveDataAction;
    SaveData* dataSaver;

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

    //linker window data
    QWidget* linkerWindow;
    QGridLayout* linkerWindowLayout;
    QAction* windowLinkerAction;
    QListWidget *linkerData;
    QPushButton *testbutton;
    void setupLinker();

    //image display data
    QWidget* imageWindow;
    QGridLayout* imageWindowLayout;
    QAction* imageDisplayAction;
    QLineEdit* selectImageIndex;
    QLabel* imageShow;
    QPushButton* confirmImageSelection;
    void setUpImageDisplay();

public slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void placeSlot();
    void clearSlot();
    void closeSlot();

    void consoleSlot();

    void linkerSlot();
    void linkerUpdateSlot(int, QString);
    void linkerClearTriggered();
    void itemActivate(QListWidgetItem * item);

    void showImage();
    void imageDisplaySlot();

};

#endif // SENSORWINDOW_H
