#include "events.h"
#include <Logger.h>

void Events::addEvent(float value)
{
    LOG_CAMPBELL() << "add event";
    events.insert(value);
    emit eventAdded();
}

void Events::removeEvent(float value)
{
    LOG_CAMPBELL() << "remove event";
    events.erase(value);
    emit eventRemoved();
}

std::set<float>::iterator Events::getIterator()
{
    return events.begin();
}

std::set<float>::iterator Events::getEnd()
{
    return events.end();
}

//returns the specified event. returns null if it doesnt exist
Events* EventContainer::getEvents(QString name, bool create)
{    
    if (events.size() > 0 && events.find(name) != events.end())
    {
        return events.at(name);
    }
    else if (create)
    {
        events[name] = new Events();
        return events[name];
    }
    return NULL;
}

void EventSystem::clearEvents()
{
    events.clear();
}

void EventSystem::saveEvents()
{
    //save stuff
}

void EventSystem::createSensor(QString name)
{
    events[name] = new Events();
}
