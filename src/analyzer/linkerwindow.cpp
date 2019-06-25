#include "linkerwindow.h"

LinkerWindow::LinkerWindow()
{

    //UI
    window = new QWidget;
    linkerWindowLayout = new QGridLayout;
    testbutton = new QPushButton("Clear Data");

    linkerData = new QListWidget();

    linkerWindowLayout->addWidget(linkerData);
    linkerWindowLayout->addWidget(testbutton);

    window->setLayout(linkerWindowLayout);
    window->setVisible(false);
    //window->setContextMenuPolicy(Qt::CustomContextMenu);

    //functionality
    connect(testbutton, SIGNAL(clicked(bool)), this, SLOT(linkerClearTriggered()));
    connect(LinkManager::instance(), SIGNAL(updated(int, QString)), this, SLOT(linkerUpdateSlot(int, QString)));
    connect(linkerData, SIGNAL(itemActivated(QListWidgetItem *)), this, SLOT(itemActivate(QListWidgetItem *)));

    openAction = new QAction(tr("&Linker"), this);
    openAction->setStatusTip("Open/Close Linker Window");
    openAction->setCheckable(true);

}

void LinkerWindow::linkerUpdateSlot(int, QString)
{
    std::map<QString, int> displayData = LinkManager::instance()->getAllData();

    linkerData->clear();
    for(std::pair<QString, int> const& result : displayData){
        QListWidgetItem* newItem = new QListWidgetItem(result.first,Q_NULLPTR, result.second);
        linkerData->addItem(newItem);
    }
}

void LinkerWindow::linkerClearTriggered()
{
    LinkManager::instance()->clearAllData();
    linkerUpdateSlot(0,"");

}

void LinkerWindow::itemActivate(QListWidgetItem *item)
{
    //saves selected data piece ot disc
    QRegExp rx("(\:)");
    QString itemIndex = item->text();
    QStringList list = itemIndex.split(rx,QString::SkipEmptyParts);

    emit onActivate(list.at(1), item->type());

    LOG_JOHN() << list.at(1);
    //display this data depending on the data

}
