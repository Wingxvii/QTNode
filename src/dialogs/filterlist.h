#ifndef FILTERLIST_H
#define FILTERLIST_H

#include <QDialog>
#include <QListView>
#include <QHBoxLayout>
#include <QListWidget>
#include "settings.h"


class FilterList : public QDialog
{
    Q_OBJECT
public:
    FilterList();
    ~FilterList();

signals:
    void sendRemove(int index);
    void sendAdd(int index);

private slots:
    void onAdd();
    void onRemove();

private:

    std::vector<QString> options;

    QListWidget listView;
    QVBoxLayout *layout;

    QPushButton *remove;
    QPushButton *add;
};

#endif // FILTERLIST_H
