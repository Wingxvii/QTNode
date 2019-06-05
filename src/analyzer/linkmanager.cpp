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
    calibList[name] = data;
    emit calibUpdated(name);
}

void LinkManager::sendData(std::shared_ptr<ImageData> data, QString name)
{
    imageList[name] = data;
    emit imageUpdated(name);
}

void LinkManager::sendData(std::shared_ptr<PointData> data, QString name)
{
    pointList[name] = data;
    emit pointUpdated(name);
}

void LinkManager::sendData(std::shared_ptr<PointsData> data, QString name)
{
    pointsList[name] = data;
    emit pointsUpdated(name);
}

void LinkManager::sendData(std::shared_ptr<VideoGraphData> data, QString name)
{
    videoGraphList[name] = data;
    emit videoUpdated(name);
}

std::shared_ptr<CalibData> LinkManager::getCalibData(QString name)
{
    if(calibList.find(name) == calibList.end()){
        return NULL;
    }
    return calibList[name];

}
std::shared_ptr<ImageData> LinkManager::getImageData(QString name)
{
    if(imageList.find(name) == imageList.end()){
        return NULL;
    }
    return imageList[name];
}

std::shared_ptr<PointData> LinkManager::getPointData(QString name)
{
    if(pointList.find(name) == pointList.end()){
        return NULL;
    }
    return pointList[name];
}

std::shared_ptr<PointsData> LinkManager::getPointsData(QString name)
{
    if(pointsList.find(name) == pointsList.end()){
        return NULL;
    }
    return pointsList[name];
}

std::shared_ptr<VideoGraphData> LinkManager::getVideoData(QString name)
{
    if(videoGraphList.find(name) == videoGraphList.end()){
        return NULL;
    }
    return videoGraphList[name];
}

void LinkManager::clearAllData()
{
    calibList.clear();
    imageList.clear();
    pointList.clear();
    pointsList.clear();
    videoGraphList.clear();
}


