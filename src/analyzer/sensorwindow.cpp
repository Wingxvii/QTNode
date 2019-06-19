#include "sensorwindow.h"
#include "ui_sensorwindow.h"
#include <Logger.h>
#include <QResizeEvent>
#include "linkmanager.h"

SensorWindow::SensorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SensorWindow)
{
    //ui
    ui->setupUi(this);
    setupLayout();

    //West
    setupLinker();

    //Central
    setupEditor();

    //East

    //South
    setupConsole();

    //North


    createActions();

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

    windowConsoleAction = new QAction(tr("&Console"), this);
    windowConsoleAction->setStatusTip("Open/Close Console Window");
    windowConsoleAction->setCheckable(true);
    connect(windowConsoleAction, SIGNAL(triggered()), this, SLOT(consoleSlot()));

    windowLinkerAction = new QAction(tr("&Linker"), this);
    windowLinkerAction->setStatusTip("Open/Close Linker Window");
    windowLinkerAction->setCheckable(true);
    connect(windowLinkerAction, SIGNAL(triggered()), this, SLOT(linkerSlot()));

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

    ui->menuWindow->addAction(windowConsoleAction);
    ui->menuWindow->addAction(windowLinkerAction);
    ui->menuWindow->addSeparator();

}

void SensorWindow::setupLayout()
{
    northWidget = new QWidget;
    northLayout = new QHBoxLayout();
    northWidget->setLayout(northLayout);

    southWidget = new QWidget;
    southLayout = new QHBoxLayout();
    southWidget->setLayout(southLayout);

    centerWidget = new QWidget;
    centerLayout = new QHBoxLayout();
    centerWidget->setLayout(centerLayout);

    ui->MainLayout->addWidget(northWidget,1,1);
    ui->MainLayout->addWidget(southWidget,3,1);
    ui->MainLayout->addWidget(centerWidget,2,1);

}

void SensorWindow::setupEditor()
{
    nodeEditorWindow = new QTabWidget();
    centerLayout->addWidget(nodeEditorWindow);

}

void SensorWindow::setupConsole()
{
    consoleWindow = new QWidget();
    consoleWindowLayout = new QGridLayout();
    consoleWindow->setLayout(consoleWindowLayout);

    consoleInput = new QLineEdit();
    consoleOutput = new QTextEdit(("<h3>JS Console </h3>"));

    consoleWindowLayout->addWidget(consoleOutput);
    consoleWindowLayout->addWidget(consoleInput);

    consoleInput->setAlignment(Qt::AlignLeft);
    consoleInput->setPlaceholderText("<< Instruction");
    consoleOutput->setReadOnly(true);

    connect(consoleInput, SIGNAL(returnPressed()), this,SLOT(consoleEnterSlot()) );

    southLayout->addWidget(consoleWindow);
    consoleWindow->setVisible(false);

    builder = new JSBuilder();
    QJSValue scriptBuilder = engine.newQObject(builder);

    engine.globalObject().setProperty("cashe", scriptBuilder);

}

void SensorWindow::setupLinker()
{
    //UI
    linkerWindow = new QWidget;
    linkerWindowLayout = new QGridLayout;
    testbutton = new QPushButton("Clear Data");

    linkerData = new QTextEdit(("<h3>Data Cashe</h3>"));
    linkerData->setReadOnly(true);

    linkerWindowLayout->addWidget(linkerData);
    linkerWindowLayout->addWidget(testbutton);

    linkerWindow->setLayout(linkerWindowLayout);
    centerLayout->addWidget(linkerWindow);
    linkerWindow->setVisible(false);

    //functionality
    connect(testbutton, SIGNAL(clicked(bool)), this, SLOT(linkerClearTriggered()));
    connect(LinkManager::instance(), SIGNAL(updated(int, QString)), this, SLOT(linkerUpdateSlot(int, QString)));

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

void SensorWindow::consoleSlot()
{
    if(windowConsoleAction->isChecked()){
        consoleWindow->setVisible(true);
    }else{
        consoleWindow->setVisible(false);
    }
}

void SensorWindow::consoleEnterSlot()
{
    consoleOutput->append(QString("<font color='red'>></font> %1").arg(consoleInput->text()));
    auto result = engine.evaluate(consoleInput->text());
    consoleOutput->append(QString("<i>%1</i><br>").arg(result.toString()));
    consoleInput->clear();

}

void SensorWindow::linkerSlot()
{
    if(windowLinkerAction->isChecked()){
        linkerWindow->setVisible(true);
    }else{
        linkerWindow->setVisible(false);
    }
}

void SensorWindow::linkerUpdateSlot(int dataIndex, QString name)
{
    std::vector<QString> displayData = LinkManager::instance()->getAllData();

    linkerData->clear();
    for(QString const& result : displayData){
        linkerData->append(QString("<font color='lightblue'><b>%1</b></font> ").arg(result));
    }
}

void SensorWindow::linkerClearTriggered()
{
    LinkManager::instance()->clearAllData();
    linkerUpdateSlot(0,"");
}
