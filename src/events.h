#ifndef EVENTS_H
#define EVENTS_H
#include <set>
#include <map>
#include <QString>
#include <QObject>

//Campbells code

class Events : public QObject
{
    Q_OBJECT
public:

    std::set<float>::iterator getIterator();
    std::set<float>::iterator getEnd();

signals:
    void eventAdded();
    void eventRemoved();

public slots:
    void addEvent(float);
    void removeEvent(float);

protected:
    float maxTime;
    std::set<float> events;
};

//This is so we can have ease of to our map
class EventContainer : public QObject
{
    Q_OBJECT
public:
    Events* getEvents(QString name, bool create = false);

signals:
    void eventUpdated(QString);

protected:
    std::map<QString, Events*> events;

};

class EventSystem : public EventContainer
{
public:
    void clearEvents();
    void saveEvents();
    void createSensor(QString name);

};
#endif // EVENTS_H
//campbell was here
