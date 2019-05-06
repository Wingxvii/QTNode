#ifndef BLACKLIST_H
#define BLACKLIST_H

#include <QDialog>
#include <QListView>
#include <QHBoxLayout>
#include <QListWidget>

class BlackList : public QDialog
{
    Q_OBJECT
public:
    BlackList();

public slots:
    void onRemove();
    void onAdd();

private:

    std::vector<QString> options;

    QListWidget listView;
    QVBoxLayout *layout;
    QHBoxLayout *buttons;

    QPushButton *remove;
    QPushButton *add;
    QLineEdit *edit;
};

#endif // BLACKLIST_H
