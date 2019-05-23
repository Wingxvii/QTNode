#ifndef SENSORWINDOW_H
#define SENSORWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "analyzer/sensormanager.h"
#include "analyzer/graphcontoller.h"

namespace Ui {
class SensorWindow;
}

class SensorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SensorWindow(QWidget *parent = nullptr);
    void resizeEvent( QResizeEvent *e );
    SensorManager* getManager();

signals:
    //signal that we want to redraw the
    void redraw();

private:
    SensorManager* m_sensorManager;
    GraphController* m_graphs;

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

    QMenu* blackBoxMenu;

public slots:
    void newSlot();
    void openSlot();
    void saveSlot();


};

#endif // SENSORWINDOW_H
