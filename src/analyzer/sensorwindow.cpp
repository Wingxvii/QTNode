#include "sensorwindow.h"
#include "ui_sensorwindow.h"
#include <Logger.h>
#include <QResizeEvent>

SensorWindow::SensorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SensorWindow)
{
    //ui
    nodeEditorWindow = new QTabWidget();

    ui->setupUi(this);
    //ui->titles->addWidget(new QLabel("testing"));
    ui->MainLayout->addWidget(nodeEditorWindow);

    createActions();

    //connections


    //setup
    //ui->MainLayout->setSizeConstraint(QLayout::SetFixedSize);
    refreshSize = 200;



}

void SensorWindow::resizeEvent(QResizeEvent *e)
{
    LOG_CAMPBELL() << "resizing";


    if (std::abs(e->size().width() - currentSize.width()) > refreshSize
            || std::abs(e->size().height() - currentSize.height()) > refreshSize )
    {
        QSize temp(std::abs(e->size().width() - currentSize.width()),
                   std::abs(e->size().height() - currentSize.height()));

        LOG_CAMPBELL() << QString::number(temp.height());
        LOG_CAMPBELL() << QString::number(temp.width());

        currentSize = e->size();
    }
}

EventContainer *SensorWindow::getContainer()
{
    return &sensorEvents;
}


void SensorWindow::createActions()
{
    fileNewAction = new QAction(tr("&New"), this);
    fileNewAction->setShortcut(QKeySequence::New);
    fileNewAction->setStatusTip("Create a new file");
    connect(fileNewAction, SIGNAL(triggered()), this, SLOT(newSlot()));

    fileOpenAction = new QAction(tr("&Open..."), this);
    fileOpenAction->setShortcut(QKeySequence::Open);
    fileOpenAction->setStatusTip("Open an existing file");
    connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(openSlot()));

    filePlaceAction = new QAction(tr("&Place"), this);
    filePlaceAction->setStatusTip("Place an existing file");
    connect(filePlaceAction, SIGNAL(triggered()), this, SLOT(placeSlot()));

    fileSaveAction = new QAction(tr("&Save"), this);
    fileSaveAction->setShortcut(QKeySequence::Save);
    fileSaveAction->setStatusTip("Save the node setup to disk");
    connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(saveSlot()));

    fileClearAction = new QAction(tr("&Clear"), this);
    fileClearAction->setStatusTip("Clear selected file");
    connect(fileClearAction, SIGNAL(triggered()), this, SLOT(clearSlot()));

    fileCloseAction = new QAction(tr("&Close"), this);
    fileCloseAction->setStatusTip("Close selected file without saving");
    connect(fileCloseAction, SIGNAL(triggered()), this, SLOT(closeSlot()));

    createMenus();
}

void SensorWindow::createMenus()
{
    ui->menuFIle->addAction(fileNewAction);
    ui->menuFIle->addAction(fileOpenAction);
    ui->menuFIle->addAction(filePlaceAction);
    ui->menuFIle->addAction(fileSaveAction);
    ui->menuFIle->addAction(fileClearAction);
    ui->menuFIle->addAction(fileCloseAction);
    ui->menuFIle->addSeparator();
}

void SensorWindow::newSlot()
{

    QString name = QInputDialog::getText(this, "Name File", "Input a name: ");

    //create the container
    NodeEditorContainer *container = new NodeEditorContainer();
    container->name = name;
    container->index = nodeWindowList.size()-1;
    container->events = new Events();                      //this doesnt do anything yet
    container->editor = new FilterNode(container->events);

    //then add a tab
    nodeEditorWindow->addTab(container->editor, container->name);

    //and push it into the list
    nodeWindowList.push_back(*container);
}

void SensorWindow::openSlot()
{

    //create the container
    NodeEditorContainer *container = new NodeEditorContainer();
    container->name = "Opening File...";
    container->index = nodeWindowList.size()-1;
    container->events = new Events();                      //this doesnt do anything yet
    container->editor = new FilterNode(container->events);

    //then add a tab
    nodeEditorWindow->addTab(container->editor, container->name);

    //and push it into the list
    nodeWindowList.push_back(*container);

    nodeEditorWindow->setCurrentIndex(nodeWindowList.size()-1);
    nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->load();
    if(nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->currentFileName != ""){
    nodeEditorWindow->setTabText(nodeEditorWindow->currentIndex(),nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->currentFileName);
    }else{
        closeSlot();
    }

}

void SensorWindow::saveSlot()
{
    nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->save();
    if(nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->currentFileName != ""){
        nodeEditorWindow->setTabText(nodeEditorWindow->currentIndex(),nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->currentFileName);
    }
}

void SensorWindow::placeSlot()
{
    nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->place();
}

void SensorWindow::clearSlot()
{
    nodeWindowList[nodeEditorWindow->currentIndex()].editor->scene->clearScene();
}

void SensorWindow::closeSlot()
{
    nodeEditorWindow->removeTab(nodeEditorWindow->currentIndex());
    nodeWindowList.removeAt(nodeEditorWindow->currentIndex());
}
