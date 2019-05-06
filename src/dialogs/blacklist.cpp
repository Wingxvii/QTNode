#include "blacklist.h"
#include <settings.h>
#include <vector>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <Logger.h>

BlackList::BlackList()
{
    LOG_CAMPBELL() << "start";

    options = ShotcutSettings::singleton().getBlackList();

    for (int i = 0; i < options.size(); i++)
    {
        listView.addItem(options.at(i));
    }

    layout = new QVBoxLayout();

    //does not like adding list view for some reason
    QWidget* test = &listView;
    layout->addWidget(test);

    setLayout(layout);

    buttons = new QHBoxLayout();

    remove = new QPushButton("remove");
    layout->addWidget(remove);

    layout->addLayout(buttons);

    add = new QPushButton("Add");
    edit = new QLineEdit();

    buttons->addWidget(add);
    buttons->addWidget(edit);
    LOG_CAMPBELL() << "end";

    connect(add, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onRemove()));
}

void BlackList::onRemove()
{
    if (options.size() > 0)
    {
        if (listView.currentIndex().isValid())
        {
            options.erase(options.begin() + listView.currentIndex().row());
            ShotcutSettings::singleton().setBlackList(options);

            listView.clear();
            for (int i = 0; i < options.size(); i++)
            {
                listView.addItem(options.at(i));
            }
        }
    }
}

void BlackList::onAdd()
{
    options.push_back(edit->text());
    ShotcutSettings::singleton().setBlackList(options);
    listView.addItem(edit->text());
}
