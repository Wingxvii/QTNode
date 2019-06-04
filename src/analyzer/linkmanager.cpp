#include "linkmanager.h"


void LinkManager::sendData(CalibData data, int index)
{
    calibList[index] = data;
}

void LinkManager::sendData(ImageData data, int index)
{
    imageList[index] = data;
}

void LinkManager::sendData(PointData data, int index)
{
    pointList[index] = data;
}

void LinkManager::sendData(PointsData data, int index)
{
    pointsList[index] = data;
}

void LinkManager::sendData(VideoGraphData data, int index)
{
    videoGraphList[index] = data;
}

CalibData LinkManager::getCalibData(int index)
{
    return calibList[index];
}

ImageData LinkManager::getImageData(int index)
{
    return imageList[index];
}

PointData LinkManager::getPointData(int index)
{
    return pointList[index];
}

PointsData LinkManager::getPointsData(int index)
{
    return pointsList[index];
}

VideoGraphData LinkManager::getVideoData(int index)
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
