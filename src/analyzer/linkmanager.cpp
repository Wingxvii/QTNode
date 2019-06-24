#include "linkmanager.h"

LinkManager* LinkManager::m_instance = 0;

LinkManager::LinkManager()
{
    calibList = std::map<QString, std::shared_ptr<CalibData>>();
    imageList = std::map<QString, std::shared_ptr<ImageData>>();
    pointList = std::map<QString, std::shared_ptr<PointData>>();
    pointsList = std::map<QString, std::shared_ptr<PointsData>>();
    videoGraphList = std::map<QString, std::shared_ptr<VideoGraphData>>();
    intList = std::map<QString, int>();
    floatList = std::map<QString, float>();
    stringList = std::map<QString, QString>();
    boolList = std::map<QString, bool>();

}

void LinkManager::multiThreadVideoSave()
{
    cv::String fileNameCV = stringListPrivate["PRIVATEfilesave"].toStdString();

    int fps = videoGraphList[stringListPrivate["PRIVATEsave"]]->getFrameRate();
    if(fps == 0){ fps = 30; }

    int width = videoGraphList[stringListPrivate["PRIVATEsave"]]->_video[1].size().width;
    int height = videoGraphList[stringListPrivate["PRIVATEsave"]]->_video[1].size().height;

    cv::VideoWriter videoOutput(fileNameCV,CV_FOURCC('M','J','P','G'), fps, cv::Size(width, height));

    for(cv::Mat const& img : videoGraphList[stringListPrivate["PRIVATEsave"]]->_video){
        videoOutput.write(img);
    }

    videoOutput.release();

}

void LinkManager::sendData(std::shared_ptr<CalibData> data, QString name)
{
    if(name.contains("PRIVATE")){
        calibListPrivate[name] = data;
    }else if(name != ""){
        calibList[name] = data;
        emit updated(0,name);
    }
}

void LinkManager::sendData(std::shared_ptr<ImageData> data, QString name)
{
    if(name.contains("PRIVATE")){
        imageListPrivate[name] = data;
    }else if(name != ""){
        imageList[name] = data;
        emit updated(1,name);
    }
}

void LinkManager::sendData(std::shared_ptr<PointData> data, QString name)
{
    if(name.contains("PRIVATE")){
        pointListPrivate[name] = data;
    }else if(name != ""){
        pointList[name] = data;
        emit updated(2,name);
    }
}

void LinkManager::sendData(std::shared_ptr<PointsData> data, QString name)
{
    if(name.contains("PRIVATE")){
        pointsListPrivate[name] = data;
    }else if(name != ""){
        pointsList[name] = data;
        emit updated(3,name);
    }
}

void LinkManager::sendData(std::shared_ptr<VideoGraphData> data, QString name)
{
    if(name.contains("PRIVATE")){
        videoGraphListPrivate[name] = data;
    }else if(name != ""){
        videoGraphList[name] = data;
        emit updated(4,name);
    }
}

void LinkManager::sendData(int data, QString name)
{
    if(name.contains("PRIVATE")){
        intListPrivate[name] = data;
    }else if(name != ""){
        intList[name] = data;
        emit updated(5,name);
    }
}

void LinkManager::sendData(float data, QString name)
{
    if(name.contains("PRIVATE")){
        floatListPrivate[name] = data;
    }else if(name != ""){
        floatList[name] = data;
        emit updated(6,name);
    }
}

void LinkManager::sendData(QString data, QString name)
{
    if(name.contains("PRIVATE")){
        stringListPrivate[name] = data;
    }else if(name != ""){
        stringList[name] = data;
        emit updated(7,name);
    }
}

void LinkManager::sendData(bool data, QString name)
{
    if(name.contains("PRIVATE")){
        boolListPrivate[name] = data;
    }else if(name != ""){
        boolList[name] = data;
        emit updated(8,name);
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
    if(intList.find(name) == intList.end()){
        return NULL;
    }
    return intList[name];

}

float LinkManager::getFloatData(QString name)
{
    if(name.contains("PRIVATE")){
        return floatListPrivate[name];
    }
    if(floatList.find(name) == floatList.end()){
        return NULL;
    }
    return floatList[name];

}

QString LinkManager::getStringData(QString name)
{
    if(name.contains("PRIVATE")){
        return stringListPrivate[name];
    }
    if(stringList.find(name) == stringList.end()){
        return NULL;
    }
    return stringList[name];

}

bool LinkManager::getBoolData(QString name)
{
    if(name.contains("PRIVATE")){
        return boolListPrivate[name];
    }
    if(boolList.find(name) == boolList.end()){
        return NULL;
    }
    return boolList[name];
}

void LinkManager::saveImageData(QString name)
{
    if(imageList[name]){
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Image"), QString(), tr("Images (*.png *.jpg)"));
        cv::String fileNameCV = fileName.toStdString();
        cv::imwrite(fileNameCV,imageList[name]->_image);
    }
}

void LinkManager::saveVideoData(QString name)
{
    //need to multithread
    if(videoGraphList[name]){
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Video"), QString(), tr("Videos (*.avi)"));

        sendData(fileName, "PRIVATEfilesave");
        sendData(name, "PRIVATEsave");
        functVidSave = QtConcurrent::run(this, &LinkManager::multiThreadVideoSave);
    }
}

void LinkManager::saveStringData(QString name)
{
    QString fileName = QFileDialog::getSaveFileName();
}


void LinkManager::clearAllData()
{
    calibList.clear();
    imageList.clear();
    pointList.clear();
    pointsList.clear();
    videoGraphList.clear();
    intList.clear();
    floatList.clear();
    stringList.clear();
    boolList.clear();
}

void LinkManager::privateClear()
{
    calibListPrivate.clear();
    imageListPrivate.clear();
    pointListPrivate.clear();
    pointsListPrivate.clear();
    videoGraphListPrivate.clear();
    intListPrivate.clear();
    floatListPrivate.clear();
    stringListPrivate.clear();
    boolListPrivate.clear();
}

std::vector<QString> LinkManager::getAllData(int x)
{
    std::vector<QString> out = std::vector<QString>();

    for(auto const& data : calibList){
        QString newString = "Calib Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : imageList){
        QString newString = "Image Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : pointList){
        QString newString = "Point Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : pointsList){
        QString newString = "Points Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : videoGraphList){
        QString newString = "Video Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : intList){
        QString newString = "Int Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : floatList){
        QString newString = "Float Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : stringList){
        QString newString = "String Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }

    for(auto const& data : boolList){
        QString newString = "Bool Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.push_back(newString);
    }



    return out;

}

