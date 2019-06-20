#include "analyzer/jsbuilder.h"
#include <QMetaObject>



void JSBuilder::sendData(const QString index, int data)
{
    LinkManager::instance()->sendData(data, index);

}

void JSBuilder::sendData(const QString index, float data)
{
    LinkManager::instance()->sendData(data, index);
}

void JSBuilder::sendData(const QString index, QString data)
{
    LinkManager::instance()->sendData(data, index);

}

void JSBuilder::sendData(const QString index, bool data)
{
    LinkManager::instance()->sendData(data, index);

}

int JSBuilder::getIntData(const QString index)
{
    return LinkManager::instance()->getIntData(index);
}

float JSBuilder::getFloatData(const QString index)
{
    return LinkManager::instance()->getFloatData(index);
}

QString JSBuilder::getStringData(const QString index)
{
    return LinkManager::instance()->getStringData(index);
}

bool JSBuilder::getBoolData(const QString index)
{
    return LinkManager::instance()->getBoolData(index);
}

void JSBuilder::saveImageData(const QString index)
{
    LinkManager::instance()->saveImageData(index);
}

void JSBuilder::saveVideoData(const QString index)
{
    LinkManager::instance()->saveVideoData(index);
}

void JSBuilder::saveStringData(const QString index)
{
    LinkManager::instance()->saveStringData(index);

}


