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

void JSBuilder::makeCalibData(const QString index, const int x, const int y, const float length)
{
    LinkManager::instance()->makeCalibData(x,y,length, index);
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

void JSBuilder::saveData(const QString index, const int indexType)
{
    LinkManager::instance()->saveData(index, indexType);
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

void JSBuilder::saveEmotionData(const QString index)
{
    LinkManager::instance()->saveEmotionData(index);
}

void JSBuilder::deleteData(const QString index, const int indexType)
{
    LinkManager::instance()->deleteData(index, indexType);
}

void JSBuilder::deleteCalibData(QString name)
{
    LinkManager::instance()->deleteCalibData(name);
}

void JSBuilder::deleteImageData(QString name)
{
    LinkManager::instance()->deleteImageData(name);

}

void JSBuilder::deletePointData(QString name)
{
    LinkManager::instance()->deletePointData(name);

}

void JSBuilder::deletePointsData(QString name)
{
    LinkManager::instance()->deletePointsData(name);

}

void JSBuilder::deleteVideoData(QString name)
{
    LinkManager::instance()->deleteVideoData(name);

}

void JSBuilder::deleteIntData(QString name)
{
    LinkManager::instance()->deleteIntData(name);

}

void JSBuilder::deleteFloatData(QString name)
{
    LinkManager::instance()->deleteFloatData(name);

}

void JSBuilder::deleteStringData(QString name)
{
    LinkManager::instance()->deleteStringData(name);

}

void JSBuilder::deleteBoolData(QString name)
{
    LinkManager::instance()->deleteBoolData(name);

}

void JSBuilder::deleteDetectionData(QString name)
{
    LinkManager::instance()->deleteDetectionData(name);
}

void JSBuilder::deleteEmotionData(QString name)
{
    LinkManager::instance()->deleteEmotionData(name);
}
void JSBuilder::cloneData(const QString index, const int indexType)
{
    LinkManager::instance()->cloneData(index, indexType);
}

void JSBuilder::cloneCalibData(QString name)
{
    LinkManager::instance()->cloneCalibData(name);
}

void JSBuilder::cloneImageData(QString name)
{
    LinkManager::instance()->cloneImageData(name);

}

void JSBuilder::clonePointData(QString name)
{
    LinkManager::instance()->clonePointData(name);

}

void JSBuilder::clonePointsData(QString name)
{
    LinkManager::instance()->clonePointsData(name);

}

void JSBuilder::cloneVideoData(QString name)
{
    LinkManager::instance()->cloneVideoData(name);

}

void JSBuilder::cloneIntData(QString name)
{
    LinkManager::instance()->cloneIntData(name);

}

void JSBuilder::cloneFloatData(QString name)
{
    LinkManager::instance()->cloneFloatData(name);

}

void JSBuilder::cloneStringData(QString name)
{
    LinkManager::instance()->cloneStringData(name);

}

void JSBuilder::cloneBoolData(QString name)
{
    LinkManager::instance()->cloneBoolData(name);

}

void JSBuilder::cloneDetectionData(QString name)
{
    LinkManager::instance()->cloneDetectionData(name);
}

void JSBuilder::cloneEmotionData(QString name)
{
    LinkManager::instance()->cloneEmotionData(name);
}

void JSBuilder::displayData(const QString index, const int indexType)
{
    LinkManager::instance()->displayData(index, indexType);
}

void JSBuilder::displayCalibData(QString name)
{
    LinkManager::instance()->displayCalibData(name);
}

void JSBuilder::displayImageData(QString name)
{
    LinkManager::instance()->displayImageData(name);

}

void JSBuilder::displayPointData(QString name)
{
    LinkManager::instance()->displayPointData(name);

}

void JSBuilder::displayPointsData(QString name)
{
    LinkManager::instance()->displayPointsData(name);

}

void JSBuilder::displayVideoData(QString name)
{
    LinkManager::instance()->displayVideoData(name);

}

void JSBuilder::displayIntData(QString name)
{
    LinkManager::instance()->displayIntData(name);

}

void JSBuilder::displayFloatData(QString name)
{
    LinkManager::instance()->displayFloatData(name);

}

void JSBuilder::displayStringData(QString name)
{
    LinkManager::instance()->displayStringData(name);

}

void JSBuilder::displayBoolData(QString name)
{
    LinkManager::instance()->displayBoolData(name);

}

void JSBuilder::displayDetectionData(QString name)
{
    LinkManager::instance()->displayDetectionData(name);
}

void JSBuilder::displayEmotionData(QString name)
{
    LinkManager::instance()->displayEmotionData(name);
}

void JSBuilder::loadImage(QString name)
{
    LinkManager::instance()->loadImage(name);
}




