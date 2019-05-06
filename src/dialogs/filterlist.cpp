#include "filterlist.h"
#include <vector>
#include <QStringListModel>
#include <QPushButton>
#include <QLineEdit>
#include <Logger.h>

FilterList::FilterList()
{
    LOG_CAMPBELL() << "start";

    ShotcutSettings::singleton().clearFilterList();
    std::vector<QString> setup;
    setup.push_back("min");
    setup.push_back("max");
    setup.push_back("range");
    ShotcutSettings::singleton().setFilterList(setup);

    options = ShotcutSettings::singleton().getFilterList();

    for (int i = 0; i < options.size(); i++)
    {
        listView.addItem(options.at(i));
    }


    layout = new QVBoxLayout();

    //does not like adding list view for some reason
    QWidget* test = &listView;
    layout->addWidget(test);

    setLayout(layout);


    remove = new QPushButton("remove");
    layout->addWidget(remove);


    add = new QPushButton("Add");
    layout->addWidget(add);
    LOG_CAMPBELL() << "end";

    connect(add, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onRemove()));
}

void FilterList::onAdd()
{
    if (listView.currentIndex().isValid())
    {
        emit sendAdd(listView.currentIndex().row());
    }
}

void FilterList::onRemove()
{
    LOG_CAMPBELL() << " Functionality not yet setup";
}

FilterList::~FilterList()
{
    LOG_CAMPBELL() << "testing";
}
