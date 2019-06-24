#include "savedata.h"
#include <QInputDialog>


SaveData::SaveData()
{
    openAction = new QAction(tr("&Save Cached Data"), this);
    openAction->setStatusTip("Open Data Saving Window");
    connect(openAction, SIGNAL(triggered()), this, SLOT(openSaveWindow()));

}

void SaveData::openSaveWindow()
{
    LOG_JOHN() << "Not implemented yet";
    //popup of this widget window
}

