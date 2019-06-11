#include "linkmanager.h"

LinkManager* LinkManager::m_instance = 0;

LinkManager::LinkManager()
{
    calibList = std::map<QString, std::shared_ptr<CalibData>>();
    imageList = std::map<QString, std::shared_ptr<ImageData>>();
    pointList = std::map<QString, std::shared_ptr<PointData>>();
    pointsList = std::map<QString, std::shared_ptr<PointsData>>();
    videoGraphList = std::map<QString, std::shared_ptr<VideoGraphData>>();

}

void LinkManager::sendData(std::shared_ptr<CalibData> data, QString name)
{
    if(name.contains("PRIVATE")){
        calibListPrivate[name] = data;
    }

    if(name != ""){
        calibList[name] = data;
        emit calibUpdated(name);
    }
}

void LinkManager::sendData(std::shared_ptr<ImageData> data, QString name)
{
    if(name.contains("PRIVATE")){
        imageListPrivate[name] = data;
    }

    if(name != ""){
        imageList[name] = data;
        emit imageUpdated(name);
    }
}

void LinkManager::sendData(std::shared_ptr<PointData> data, QString name)
{
    if(name.contains("PRIVATE")){
        pointListPrivate[name] = data;
    }
    if(name != ""){
        pointList[name] = data;
        emit pointUpdated(name);
    }
}

void LinkManager::sendData(std::shared_ptr<PointsData> data, QString name)
{
    if(name.contains("PRIVATE")){
        pointsListPrivate[name] = data;
    }
        if(name != ""){
    pointsList[name] = data;
    emit pointsUpdated(name);
        }
}

void LinkManager::sendData(std::shared_ptr<VideoGraphData> data, QString name)
{
    if(name.contains("PRIVATE")){
        videoGraphListPrivate[name] = data;
    }
        if(name != ""){
    videoGraphList[name] = data;
    emit videoUpdated(name);
        }
}

void LinkManager::sendData(int data, QString name)
{
    if(name.contains("PRIVATE")){
        intListPrivate[name] = data;
    }
}

void LinkManager::sendData(QString data, QString name)
{
    if(name.contains("PRIVATE")){
        stringListPrivate[name] = data;
    }
}

std::shared_ptr<CalibData> LinkManager::getCalibData(QString name)
{
    if(name.contains("PRIVATE")){
        return calibListPrivate[name];
    }
    if(calibList.find(name) == calibList.end()){
        return NULL;
    }
    return calibList[name];

}
std::shared_ptr<ImageData> LinkManager::getImageData(QString name)
{
    if(name.contains("PRIVATE")){
        return imageListPrivate[name];
    }
    if(imageList.find(name) == imageList.end()){
        return NULL;
    }
    return imageList[name];
}

std::shared_ptr<PointData> LinkManager::getPointData(QString name)
{
    if(name.contains("PRIVATE")){
        return pointListPrivate[name];
    }
    if(pointList.find(name) == pointList.end()){
        return NULL;
    }
    return pointList[name];
}

std::shared_ptr<PointsData> LinkManager::getPointsData(QString name)
{
    if(name.contains("PRIVATE")){
        return pointsListPrivate[name];
    }
    if(pointsList.find(name) == pointsList.end()){
        return NULL;
    }
    return pointsList[name];
}

std::shared_ptr<VideoGraphData> LinkManager::getVideoData(QString name)
{
    if(name.contains("PRIVATE")){
        return videoGraphListPrivate[name];
    }
    if(videoGraphList.find(name) == videoGraphList.end()){
        return NULL;
    }
    return videoGraphList[name];
}

int LinkManager::getIntData(QString name)
{
    if(name.contains("PRIVATE")){
        return intListPrivate[name];
    }

}

QString LinkManager::getNameData(QString name)
{
    if(name.contains("PRIVATE")){
        return stringListPrivate[name];
    }

}

void LinkManager::clearAllData()
{
    calibList.clear();
    imageList.clear();
    pointList.clear();
    pointsList.clear();
    videoGraphList.clear();
}

void LinkManager::privateClear()
{
    calibListPrivate.clear();
    imageListPrivate.clear();
    pointListPrivate.clear();
    pointsListPrivate.clear();
    videoGraphListPrivate.clear();
    intListPrivate.clear();
    stringListPrivate.clear();
}


