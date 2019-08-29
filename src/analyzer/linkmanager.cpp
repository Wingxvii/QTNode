#include "linkmanager.h"

LinkManager* LinkManager::m_instance = 0;

LinkManager::LinkManager()
{
    calibList = std::map<QString, std::shared_ptr<CalibData>>();
    imageList = std::map<QString, std::shared_ptr<ImageData>>();
    pointList = std::map<QString, std::shared_ptr<PointData>>();
    pointsList = std::map<QString, std::shared_ptr<PointsData>>();
    videoGraphList = std::map<QString, std::shared_ptr<VideoGraphData>>();
    emotionDataList = std::map<QString, std::shared_ptr<EmotionData>>();

    intList = std::map<QString, int>();
    floatList = std::map<QString, float>();
    stringList = std::map<QString, QString>();
    boolList = std::map<QString, bool>();
    detectionBoxesList = std::map<QString, std::shared_ptr<DetectionBoxesData>>();

}

void LinkManager::multiThreadVideoSave()
{
    cv::String fileNameCV = stringListPrivate["PRIVATEfilesave"].toStdString();

    int fps = videoGraphList[stringListPrivate["PRIVATEsave"]]->getFrameRate();
    if(fps == 0){ fps = 30; }

    int width = videoGraphList[stringListPrivate["PRIVATEsave"]]->_video[1].size().width;
    int height = videoGraphList[stringListPrivate["PRIVATEsave"]]->_video[1].size().height;

    cv::VideoWriter videoOutput(fileNameCV,cv::VideoWriter::fourcc('M','J','P','G'), fps, cv::Size(width, height));

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

void LinkManager::sendData(std::shared_ptr<EmotionData> data, QString name)
{
    if(name.contains("PRIVATE")){
        emotionDataListPrivate[name] = data;
    }else if(name != ""){
        emotionDataList[name] = data;
        emit updated(10,name);
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

void LinkManager::sendData(std::shared_ptr<DetectionBoxesData> data, QString name)
{
    if(name.contains("PRIVATE")){
        detectionBoxesListPrivate[name] = data;
    }else if(name != ""){
        detectionBoxesList[name] = data;
        emit updated(9,name);
    }

}

void LinkManager::makeCalibData(int boardX, int boardY, float boardLength, QString name)
{
    std::shared_ptr<CalibData> newCalib = std::make_shared<CalibData>();
    newCalib->setSizeXData(boardX);
    newCalib->setSizeYData(boardY);
    newCalib->setLengthData(boardLength);
    newCalib->ready();

    sendData(newCalib, name);

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

std::shared_ptr<EmotionData> LinkManager::getEmotionData(QString name)
{
    if(name.contains("PRIVATE")){
        return emotionDataListPrivate[name];
    }
    if(emotionDataList.find(name) == emotionDataList.end()){
        return NULL;
    }
    return emotionDataList[name];
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

std::shared_ptr<DetectionBoxesData> LinkManager::getDetectionData(QString name)
{
    if(name.contains("PRIVATE")){
        return detectionBoxesListPrivate[name];
    }
    if(detectionBoxesList.find(name) == detectionBoxesList.end()){
        return NULL;
    }
    return detectionBoxesList[name];
}

void LinkManager::saveData(QString name, int type)
{
    switch(type){
    case 0:
        //calib
        LOG_JOHN() << "Feature does not exist";
        break;
    case 1:
        //image
        saveImageData(name);
        break;
    case 2:
        //point
        LOG_JOHN() << "Feature does not exist";
        break;
    case 3:
        //points
        LOG_JOHN() << "Feature does not exist";
        break;
    case 4:
        //video
        saveVideoData(name);
        break;
    case 5:
        //int
        LOG_JOHN() << "Feature does not exist";
        break;
    case 6:
        //float
        LOG_JOHN() << "Feature does not exist";
        break;
    case 7:
        //string
        saveStringData(name);
        break;
    case 8:
        //bool
        LOG_JOHN() << "Feature does not exist";
        break;
    case 9:
        //detection boxes
        LOG_JOHN() << "Feature does not exist";
        break;
    case 10:
        //emotion boxes
        LOG_JOHN() << "Feature does not exist";
        break;

    default:
        //no type
        LOG_JOHN() << "Type does not exist";
        LOG_JOHN() << "Feature does not exist";
        break;
    }

}

void LinkManager::saveImageData(QString name)
{
    if(imageList[name]){
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Image"), QString(), tr("Images (*.png *.jpg)"));
        cv::String fileNameCV = fileName.toStdString();
        cv::imwrite(fileNameCV,imageList[name]->_image);
    }else{
        LOG_JOHN() << "DATA NOT FOUND";
    }
}

void LinkManager::saveVideoData(QString name)
{
    if(videoGraphList[name]){
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Video"), QString(), tr("Videos (*.avi)"));

        sendData(fileName, "PRIVATEfilesave");
        sendData(name, "PRIVATEsave");
        functVidSave = QtConcurrent::run(this, &LinkManager::multiThreadVideoSave);
    }else{
        LOG_JOHN() << "DATA NOT FOUND";
    }
}

void LinkManager::saveEmotionData(QString name)
{
    if(emotionDataList[name]){
        if(!emotionDataList[name]->_valuePercentages.empty()){
            QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Text"), QString(), tr("Text (*.txt)"));
            QFile out(fileName);

            out.open(QIODevice::ReadWrite | QIODevice::Text);
            QTextStream stream(&out);

            for(int counter = 0; counter <emotionDataList[name]->_valuePercentages.size(); counter++ ){
                QString keyframeInfo = "Frame Number: " + QString::number(emotionDataList[name]->frameNumbers[counter]) + ", ";
                for(std::pair<int, float> pairing : emotionDataList[name]->_valuePercentages[counter]){
                    keyframeInfo = keyframeInfo + QString::number(pairing.first) + ": "  + QString::number(pairing.second) + "; ";
                }
                stream << keyframeInfo<< endl;
            }

            out.close();
            }else{
            LOG_JOHN() << "DATA NOT FOUND";
        }
    }else{
        LOG_JOHN() << "DATA NOT FOUND";
    }
}

void LinkManager::saveStringData(QString name)
{
    if(stringList[name] != NULL){
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Save Text"), QString(), tr("Text (*.txt)"));
    QFile out(fileName);

    out.open(QIODevice::WriteOnly);
    QTextStream stream(&out);
    stream << stringList[name];
    out.close();
    }else{
        LOG_JOHN() << "DATA NOT FOUND";
    }

}

void LinkManager::deleteData(QString name, int type)
{
    switch(type){
    case 0:
        //calib
        deleteCalibData(name);
        break;
    case 1:
        //image
        deleteImageData(name);
        break;
    case 2:
        //point
        deletePointData(name);
        break;
    case 3:
        //points
        deletePointsData(name);
        break;
    case 4:
        //video
        deleteVideoData(name);
        break;
    case 5:
        //int
        deleteIntData(name);
        break;
    case 6:
        //float
        deleteFloatData(name);
        break;
    case 7:
        //string
    deleteStringData(name);
        break;
    case 8:
        //bool
    deleteBoolData(name);
        break;
    case 9://detection boxes
        deleteDetectionData(name);
        break;
    case 10://emotions
        deleteEmotionData(name);
    default:
        //no type
        LOG_JOHN() << "Type does not exist";
        LOG_JOHN() << "Feature does not exist";
        break;
    }

}

void LinkManager::deleteCalibData(QString name)
{
    if(calibList[name]){
        calibList.erase(name);
        LOG_JOHN() << "Deleted Calib Data";
        emit updated(0, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::deleteImageData(QString name)
{
    if(imageList[name]){
        imageList.erase(name);
        LOG_JOHN() << "Deleted Image Data";
        emit updated(1, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deletePointData(QString name)
{
    if(pointList[name]){
        pointList.erase(name);
        LOG_JOHN() << "Deleted Point Data";
        emit updated(2, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deletePointsData(QString name)
{
    if(pointsList[name]){
        pointsList.erase(name);
        LOG_JOHN() << "Deleted Points Data";
        emit updated(3, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deleteVideoData(QString name)
{
    if(videoGraphList[name]){
        videoGraphList.erase(name);
        LOG_JOHN() << "Deleted Video Data";
        emit updated(4, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deleteEmotionData(QString name)
{
    if(emotionDataList[name]){
        emotionDataList.erase(name);
        LOG_JOHN() << "Deleted Emotion Data";
        emit updated(10, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::deleteIntData(QString name)
{
    if(intList[name]){
        intList.erase(name);
        LOG_JOHN() << "Deleted Int Data";
        emit updated(5, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deleteFloatData(QString name)
{
    if(floatList[name]){
        floatList.erase(name);
        LOG_JOHN() << "Deleted Float Data";
        emit updated(6, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::deleteStringData(QString name)
{
    if(stringList[name] != NULL){
        stringList.erase(name);
        LOG_JOHN() << "Deleted String Data";
        emit updated(7, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::deleteBoolData(QString name)
{
    if(boolList[name]){
        boolList.erase(name);
        LOG_JOHN() << "Deleted Bool Data";
        emit updated(8, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::deleteDetectionData(QString name)
{
    if(detectionBoxesList[name]){
        detectionBoxesList.erase(name);
        LOG_JOHN() << "Deleted Detection Data";
        emit updated(9, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::cloneData(QString name, int type)
{
    switch(type){
    case 0:
        //calib
        cloneCalibData(name);
        break;
    case 1:
        //image
        cloneImageData(name);
        break;
    case 2:
        //point
        clonePointData(name);
        break;
    case 3:
        //points
        clonePointsData(name);
        break;
    case 4:
        //video
        cloneVideoData(name);
        break;
    case 5:
        //int
        cloneIntData(name);
        break;
    case 6:
        //float
        cloneFloatData(name);
        break;
    case 7:
        //string
    cloneStringData(name);
        break;
    case 8:
        //bool
    cloneBoolData(name);
        break;
    case 9:
        //detection boxes
    cloneDetectionData(name);
        break;
    case 10:
        //emotion data
    cloneEmotionData(name);
        break;

    default:
        //no type
        LOG_JOHN() << "Type does not exist";
        LOG_JOHN() << "Feature does not exist";
        break;
    }

}

void LinkManager::cloneCalibData(QString name)
{
    if(calibList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(calibList[name],newName);
        LOG_JOHN() << "cloned Calib Data";
        emit updated(0, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::cloneImageData(QString name)
{
    if(imageList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(imageList[name],newName);
        LOG_JOHN() << "cloned Image Data";
        emit updated(1, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::clonePointData(QString name)
{
    if(pointList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(pointList[name],newName);
        LOG_JOHN() << "cloned Point Data";
        emit updated(2, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::clonePointsData(QString name)
{
    if(pointsList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(pointsList[name],newName);
        LOG_JOHN() << "cloned Points Data";
        emit updated(3, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::cloneVideoData(QString name)
{
    if(videoGraphList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(videoGraphList[name],newName);
        LOG_JOHN() << "cloned Video Data";
        emit updated(4, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::cloneEmotionData(QString name)
{
    if(emotionDataList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(emotionDataList[name],newName);
        LOG_JOHN() << "cloned Emotion Data";
        emit updated(4, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::cloneIntData(QString name)
{
    if(intList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(intList[name],newName);
        LOG_JOHN() << "cloned Int Data";
        emit updated(5, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::cloneFloatData(QString name)
{
    if(floatList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(floatList[name],newName);
        LOG_JOHN() << "cloned Float Data";
        emit updated(6, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::cloneStringData(QString name)
{
    if(stringList[name] != NULL){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(stringList[name],newName);
        LOG_JOHN() << "cloned String Data";
        emit updated(7, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::cloneBoolData(QString name)
{
    if(boolList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(boolList[name],newName);
        LOG_JOHN() << "cloned Bool Data";
        emit updated(8, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::cloneDetectionData(QString name)
{
    if(detectionBoxesList[name]){
        QString newName = QInputDialog::getText(Q_NULLPTR, "Input Name Index", "Clone Name Index:");
        sendData(detectionBoxesList[name],newName);
        LOG_JOHN() << "cloned Detection Data";
        emit updated(9, name);
    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::displayData(QString name, int type)
{
    switch(type){
    case 0:
        //calib
        displayCalibData(name);
        break;
    case 1:
        //image
        displayImageData(name);
        break;
    case 2:
        //point
        displayPointData(name);
        break;
    case 3:
        //points
        displayPointsData(name);
        break;
    case 4:
        //video
        displayVideoData(name);
        break;
    case 5:
        //int
        displayIntData(name);
        break;
    case 6:
        //float
        displayFloatData(name);
        break;
    case 7:
        //string
        displayStringData(name);
        break;
    case 8:
        //bool
        displayBoolData(name);
        break;
    case 9:
        //detection boxes
        displayDetectionData(name);
        break;
    case 10:
        //emotion
        displayEmotionData(name);
        break;
    default:
        //no type
        LOG_JOHN() << "Type does not exist";
        LOG_JOHN() << "Feature does not exist";
        break;
    }

}

void LinkManager::displayCalibData(QString name)
{
    if(calibList[name]){
        LOG_JOHN() << "display Calib Data";

        QString display = "Calib Data: \nLength: ";
        display.append(QString::number(calibList[name]->lengthData()));
        display.append("\nSize: ");
        display.append(QString::number(calibList[name]->sizeData().width));
        display.append(" by ");
        display.append(QString::number(calibList[name]->sizeData().height));

        QMessageBox::information(Q_NULLPTR, "Calib Display",display);

    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::displayImageData(QString name)
{
    if(imageList[name]){
        LOG_JOHN() << "display Image Data";

        cv::namedWindow("Display");
        cv::imshow("Display Image", imageList[name]->_image);
        cv::waitKey(0);

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayPointData(QString name)
{
    if(pointList[name]){
        LOG_JOHN() << "display Point Data";
        LOG_JOHN() << "Feature does not exist";

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayPointsData(QString name)
{
    if(pointsList[name]){
        LOG_JOHN() << "display Points Data";
        LOG_JOHN() << "Feature does not exist";

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayVideoData(QString name)
{
    if(videoGraphList[name]){
        LOG_JOHN() << "display Video Data";
        cv::namedWindow("DisplayLink");

        int frameRate = videoGraphList[name]->getFrameRate();

        if (frameRate == 0)
        {
            frameRate = 30;
        }

        auto frames = videoGraphList[name]->data();

        for (int it = 0; it < frames.size(); it++)
        {
            //LOG_DEBUG() << QString::number(cv::getWindowProperty("Display", cv::WND_PROP_VISIBLE));

            cv::imshow("Display", frames[it]);
            cv::waitKey( 1000 / frameRate );
            if(cv::getWindowProperty("Display", cv::WND_PROP_VISIBLE) == 0){
                //cv::destroyWindow("Display");
                break;
            }
        }
        cv::destroyWindow("Display");

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayEmotionData(QString name)
{
    if(pointList[name]){
        LOG_JOHN() << "display Point Data";
        LOG_JOHN() << "Feature does not exist";

    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::displayIntData(QString name)
{
    if(intList[name]){
        LOG_JOHN() << "display Int Data";
        QString display = "Int Data: \nLength: ";
        display.append(QString::number(intList[name]));

        QMessageBox::information(Q_NULLPTR, "Int Display",display);

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayFloatData(QString name)
{
    if(floatList[name]){
        LOG_JOHN() << "display Float Data";
        QString display = "Float Data: \nLength: ";
        display.append(QString::number(intList[name]));

        QMessageBox::information(Q_NULLPTR, "Int Display",display);

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::displayStringData(QString name)
{
    if(stringList[name] != NULL){
        LOG_JOHN() << "display String Data";
        QString display = "String Data: \nLength: ";
        display.append(stringList[name]);

        QMessageBox::information(Q_NULLPTR, "Int Display",display);

    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::displayBoolData(QString name)
{
    if(boolList[name]){
        LOG_JOHN() << "display Bool Data";
        QString display;
        if(boolList[name]){
            display.append("True");
        }else{
            display.append("False");

        }

        QMessageBox::information(Q_NULLPTR, "Int Display",display);

    }else{
        LOG_JOHN() << "Data Not Found";
    }
}

void LinkManager::displayDetectionData(QString name)
{
    if(detectionBoxesList[name]){
        LOG_JOHN() << "display detection boxes Data";
        LOG_JOHN() << "Feature does not exist";

    }else{
        LOG_JOHN() << "Data Not Found";
    }

}

void LinkManager::loadImage(QString name)
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Choose Image"), "");
    std::shared_ptr<ImageData> newImage = std::make_shared<ImageData>();
    newImage->_image = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);
    newImage->ready();
    sendData(newImage, name);

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
    detectionBoxesList.clear();
    emotionDataList.clear();
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
    detectionBoxesListPrivate.clear();
    emotionDataListPrivate.clear();
}

std::map<QString, int> LinkManager::getAllData(int x)
{
    std::map<QString, int> out = std::map<QString, int>();

    for(auto const& data : calibList){
        QString newString = "Calib Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 0));
    }

    for(auto const& data : imageList){
        QString newString = "Image Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 1));
    }

    for(auto const& data : pointList){
        QString newString = "Point Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 2));
    }

    for(auto const& data : pointsList){
        QString newString = "Points Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 3));
    }

    for(auto const& data : videoGraphList){
        QString newString = "Video Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 4));
    }

    for(auto const& data : intList){
        QString newString = "Int Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 5));
    }

    for(auto const& data : floatList){
        QString newString = "Float Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 6));
    }

    for(auto const& data : stringList){
        QString newString = "String Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 7));
    }

    for(auto const& data : boolList){
        QString newString = "Bool Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 8));
    }
    for(auto const& data : detectionBoxesList){
        QString newString = "Detection Boxes Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 9));
    }
    for(auto const& data : emotionDataList){
        QString newString = "Emotion Data at [:";
        newString.append(data.first);
        newString.append(":]");
        out.insert(std::pair<QString, int>(newString, 10));
    }

    return out;

}

