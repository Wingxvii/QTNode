#include "trackinglist.h"

TrackingList::TrackingList()
{
    layout = new QGridLayout;
    window = new QWidget;

    allPoints= new QListWidget();
    allPoints->setSelectionMode(QAbstractItemView::SingleSelection);

    deleteButton = new QPushButton("Delete");
    addButton = new QPushButton("Add...");
    editButton = new QPushButton("Edit...");
    reGenButton = new QPushButton("Re-Generate Points");
    clearButton = new QPushButton("Clear");
    progressBar = new QLabel("Inactive");

    addWindow = new AddFeature();

    pointsOut = std::make_shared<PointData>();

    connect(deleteButton, SIGNAL(clicked(bool)), this , SLOT(onDelete()));
    connect(addButton, SIGNAL(clicked(bool)), addWindow , SLOT(openWindow()));
    connect(addWindow, SIGNAL(sendPoint(int, int, QString)), this, SLOT(onAdd(int, int, QString)));
    connect(addWindow, SIGNAL(sendEdit(int, int, int, QString)), this, SLOT(onEditRecieve(int, int, int, QString)));
    connect(editButton, SIGNAL(clicked(bool)), this , SLOT(onEdit()));
    connect(reGenButton, SIGNAL(clicked(bool)), this , SLOT(onRegen()));
    connect(clearButton, SIGNAL(clicked(bool)), this , SLOT(onClear()));
    connect(&functWatcher, SIGNAL(finished()), this, SLOT(multiThreadedFinished()));

    layout->addWidget(allPoints,1,1,5,3);
    layout->addWidget(deleteButton,6,1);
    layout->addWidget(addButton,7,2);
    layout->addWidget(editButton,6,2);
    layout->addWidget(reGenButton,7,1);
    layout->addWidget(clearButton,6,3);
    layout->addWidget(progressBar,7,3);

    window->setLayout(layout);

    buildContextWindow();

}

unsigned int TrackingList::nPorts(QtNodes::PortType portType) const
{
    unsigned int result = 1;

    switch(portType){
    case PortType::In:
        result = 1;
        break;
    case PortType::Out:
        result = 1;
        break;
    default:
        break;
    }
    return result;

}

QtNodes::NodeDataType TrackingList::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
            return PointData().type();
}

std::shared_ptr<QtNodes::NodeData> TrackingList::outData(QtNodes::PortIndex port)
{
        return pointsOut;

}

void TrackingList::setInData(std::shared_ptr<QtNodes::NodeData> data, int location)
{
    switch(location){
    case 0:
    pointsIn = std::dynamic_pointer_cast<PointData>(data);
        if(pointsIn){
            modelValidationState = NodeValidationState::Valid;
            modelValidationError = QString();
            preCheck();
        }
       else{
          modelValidationState = NodeValidationState::Warning;
          modelValidationError = QStringLiteral("Missing or incorrect inputs");
        }
    break;
    }
}

QtNodes::NodeValidationState TrackingList::validationState() const
{
    return modelValidationState;
}

QString TrackingList::validationMessage() const
{
    return modelValidationError;
}

void TrackingList::processData()
{
    progressBar->setText("Processing...");

    funct = QtConcurrent::run(this, &TrackingList::multiThreadedProcess);
    functWatcher.setFuture(funct);
}

void TrackingList::preCheck()
{
    if(pointsIn && pointsIn->isReady && active){
        processData();
    }
}

void TrackingList::ShowContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"));

    QAction activateAction("Activate", this);
    QAction deactivateAction("Deactivate", this);

    connect(&activateAction, SIGNAL(triggered()), this, SLOT(activate()));
    connect(&deactivateAction, SIGNAL(triggered()), this, SLOT(deactivate()));
    contextMenu.addAction(&activateAction);
    contextMenu.addAction(&deactivateAction);

    contextMenu.exec(window->mapToGlobal(pos));
}

void TrackingList::multiThreadedProcess()
{
    for(int counter = 0; counter < pointsIn->_pointList.size(); counter++){
        bool found = false;
        cv::Point2f temp = pointsIn->_pointList[counter];
        for(cv::Point2f search : pointsOut->_pointList){
            if(temp.x == search.x &&temp.y == search.y){
                found = true;
            }

        }
        if(!found){
        pointsOut->_pointList.push_back(temp);
        pointsOut->_names.push_back(pointsIn->_names[counter]);
        }
    }
}

void TrackingList::multiThreadedFinished()
{
    allPoints->clear();

    //send points to list
    for(int counter = 0; counter < pointsOut->_pointList.size(); counter++){
        QString tempStr = pointsOut->_names[counter] + " at (" + QString::number(pointsOut->_pointList[counter].x) +
                "," + QString::number(pointsOut->_pointList[counter].y) + ")";

        allPoints->addItem(tempStr);
        LOG_JOHN() << "Sent Point";
    }
    LOG_JOHN() << pointsOut->_pointList.size();

    progressBar->setText("Finished");
    pointsOut->ready();
    emit dataUpdated(0);

}


void TrackingList::onDelete(){
    pointsOut->_pointList.erase(pointsOut->_pointList.begin() + allPoints->currentRow());
    pointsOut->_names.erase(pointsOut->_names.begin() + allPoints->currentRow());
    multiThreadedFinished();
}

void TrackingList::onAdd(int x, int y, QString name)
{
    pointsOut->_pointList.push_back(cv::Point2f(x,y));
    pointsOut->_names.push_back(name);
    multiThreadedFinished();
}

void TrackingList::onEditRecieve(int row, int x,int y, QString name)
{
    pointsOut->_pointList[row] = cv::Point2f(x,y);
    pointsOut->_names[row] = name;
    multiThreadedFinished();
}

void TrackingList::onEdit(){
    if(allPoints->currentRow() != -1){
    addWindow->onEdit(allPoints->currentRow(), pointsOut->_pointList[allPoints->currentRow()].x,
            pointsOut->_pointList[allPoints->currentRow()].y, pointsOut->_names[allPoints->currentRow()]);
    }
}

void TrackingList::onRegen(){
    preCheck();
}
void TrackingList::onClear(){
    pointsOut->_names.clear();
    pointsOut->_pointList.clear();
    multiThreadedFinished();
}
