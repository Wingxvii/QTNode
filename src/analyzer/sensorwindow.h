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
    QMenu* fileMenu;

    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* filePlaceAction;
    QAction* fileClearAction;
    QAction* fileCloseAction;

private: //replace sensormanager

    QTabWidget* nodeEditorWindow;
    QList<NodeEditorContainer> nodeWindowList;

    QWidget* leftDataWindow;
    QBoxLayout* leftDataWindowLayout;

public slots:
    void newSlot();
    void openSlot();
    void saveSlot();
    void placeSlot();
    void clearSlot();
    void closeSlot();


};

#endif // SENSORWINDOW_H
