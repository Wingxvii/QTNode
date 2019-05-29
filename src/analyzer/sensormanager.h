//Class made by campbell
//Used to load sensor data

#ifndef SENSORDOCK_H
#define SENSORDOCK_H

#include <QWidget>
#include <QJsonArray>
#include <QStringListModel>
#include <vector>
#include <QPushButton>
#include <QListView>
#include <QListWidget>
#include <QVBoxLayout>

#include "events.h"

class SensorManager : public QWidget
{
    Q_OBJECT

public:
    explicit SensorManager(QWidget *parent = 0);
    EventContainer* getContainer();

private:
    void parseSensors();
    void openDialog(double time);
    void clearList();
    bool checkBlacklist(QString);

    QJsonArray sensorData;
    QStringListModel *model;

    double videoTime;
    double sensorTime;
    double sensorStartTime;

    EventSystem sensorEvents;
    std::vector<QString> blackList;

public slots:
    void updateEvents(QString, std::vector<double>);
    void saveEvents();
    void videoLength(double);
    void createNew();

private slots:

signals:
    void sendSensorDuration(double time);
    void createGraphDoc(QString name, QJsonArray data);
    void createVirtual(QString);
    void createEvents(QString);

    void graphOpened(QString name);
    void graphClosed(QString name);
    void addEvent(QString);
    void removeEvent(QString);
    void clear();
    void clearEvents();

    void sendEvents(QString, std::vector<double>);


private slots:
    void on_open_clicked();
    void on_addevent_clicked();
    void on_listView_doubleClicked(const QModelIndex& index);
    void on_virtual_clicked(const QModelIndex& index);
    void on_events_Clicked(const QModelIndex& index);
    void on_blackList_clicked();
    void on_events_closed(QString eventName);

public:
    //ui
    QVBoxLayout* layout;

    QPushButton* open;
    QPushButton* blacklist;

    QListView * orignals;
    int orignalsIndex;

    QListWidget * virtualSource;
    int virtualIndex;

    QListWidget * events;
    int eventsIndex;

    QListWidget * nodePackages;
    int packagesIndex;

};

#endif // SENSORDOCK_H
