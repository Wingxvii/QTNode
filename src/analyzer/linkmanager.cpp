#include "linkmanager.h"

LinkManager* LinkManager::m_instance = 0;

LinkManager::LinkManager()
{
    calibList = QList<std::shared_ptr<CalibData>>();
    imageList = QList<std::shared_ptr<ImageData>>();
    pointList = QList<std::shared_ptr<PointData>>();
    pointsList = QList<std::shared_ptr<PointsData>>();
    videoGraphList = QList<std::shared_ptr<VideoGraphData>>();

}

void LinkManager::sendData(std::shared_ptr<CalibData> data, int index)
{
    calibList[index] = data;
    emit calibUpdated(index);
}

void LinkManager::sendData(std::shared_ptr<ImageData> data, int index)
{
    imageList[index] = data;
    emit imageUpdated(index);
}

void LinkManager::sendData(std::shared_ptr<PointData> data, int index)
{
    pointList[index] = data;
    emit pointUpdated(index);
}

void LinkManager::sendData(std::shared_ptr<PointsData> data, int index)
{
    pointsList[index] = data;
    emit pointsUpdated(index);
}

void LinkManager::sendData(std::shared_ptr<VideoGraphData> data, int index)
{
    videoGraphList[index] = data;
    emit videoUpdated(index);
}

std::shared_ptr<CalibData> LinkManager::getCalibData(int index)
{
    return calibList[index];
}

std::shared_ptr<ImageData> LinkManager::getImageData(int index)
{
    return imageList[index];
}

std::shared_ptr<PointData> LinkManager::getPointData(int index)
{
    return pointList[index];
}

std::shared_ptr<PointsData> LinkManager::getPointsData(int index)
{
    return pointsList[index];
}

std::shared_ptr<VideoGraphData> LinkManager::getVideoData(int index)
{
    return videoGraphList[index];
}

void LinkManager::clearAllData()
{
    calibList.clear();
    imageList.clear();
    pointList.clear();
    pointsList.clear();
    videoGraphList.clear();
}


