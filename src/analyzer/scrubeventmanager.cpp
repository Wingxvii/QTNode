//Campbell Hamilton
#include "scrubeventmanager.h"
#include <QPen>
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <Logger.h>
#include <QColorDialog>
#include <QInputDialog>
#include <QAbstractButton>

#ifndef CLAMP
#define CLAMP(x, min, max) (((x) < (min))? (min) : ((x) > (max))? (max) : (x))
#endif


ScrubEventManager::ScrubEventManager(EventContainer* container, QWidget *parent) : QGridLayout(parent)
{
    currentIndex = 1;
    setColumnMinimumWidth(3, 1);
    setColumnMinimumWidth(2, 1);
    setColumnStretch(0, 0);
    setColumnStretch(1, 5);
    setColumnStretch(2, 0);
    setColumnStretch(3, 0);
    setSizeConstraint(SetMinimumSize);
    save = new QPushButton("S");
    connect(save, SIGNAL(clicked(bool)), this, SLOT(saveData()));

    this->container = container;
    data.fps = 1;
}

void ScrubEventManager::setFPS(int fps)
{
    data.fps = fps;
}

void ScrubEventManager::setMargin(int margin)
{
    data.margin = margin;
}
void ScrubEventManager::setSelectionSize(int size)
{
    data.selectionSize = size;
}

void ScrubEventManager::setTime(int position)
{
    data.time = position;
    updates();
    if (data.time > maxDuration)
    {
        data.time = maxDuration;
    }
}

void ScrubEventManager::addEventToBar(QString name)
{
    LOG_CAMPBELL() << "begin";
    auto it = sensors.find(name);

    //if it doesnt exist add it
    if (it == sensors.end())
    {
        LOG_CAMPBELL() << "events added to bar";
        addSensor(name);
        sensors[name]->updatePixmap();

    }
    else
    {
        LOG_CAMPBELL() << "shit dont work";
        sensors[name]->showAll();
    }

    //sensors[name]->addEvent();
    LOG_CAMPBELL() << "end " + QString::number(data.time);
}

void ScrubEventManager::addSensor(QString name)
{
    if (sensors.find(name) == sensors.end())
    {
        LOG_CAMPBELL() << "add sensor";
        SensorBar* sensor = new SensorBar(&data, container->getEvents(name));
        sensor->name = new QLabel(name);
        sensor->event = new QPushButton("E");
        sensor->close = new QPushButton("X");
        sensor->options = new QPushButton("O");
        sensor->shape = new QPushButton("S");


        sensor->close->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sensor->close->setMinimumSize(1, 1);

        int buttonsize = data.selectionSize * 1.5;
        sensor->close->setFixedSize(buttonsize, buttonsize);
        sensor->event->setFixedSize(buttonsize, buttonsize);
        sensor->options->setFixedSize(buttonsize, buttonsize);
        sensor->shape->setFixedSize(buttonsize, buttonsize);

        //Connecting buttons
        connect(sensor->event, SIGNAL(clicked(bool)), sensor, SIGNAL(eventClicked()));
        QObject::connect(sensor, &SensorBar::eventClicked, [&, sensor]()
        {
            LOG_CAMPBELL() << "Time: " + QString::number(data.time);
            LOG_CAMPBELL() << "fps: " + QString::number(data.fps);
            LOG_CAMPBELL() << "Event:" + QString::number((float)data.time / (float)data.fps);
            container->getEvents(sensor->name->text())->addEvent((float)data.time / (float)data.fps);
        });

        QObject::connect(container->getEvents(name), &Events::eventAdded, [&, sensor]()
        {
            updatePixmap(sensor->name->text());
        });

        QObject::connect(container->getEvents(name), &Events::eventRemoved, [&, sensor]()
        {
            updatePixmap(sensor->name->text());
        });

        //connect()

        connect(sensor->close, SIGNAL(clicked(bool)), sensor, SLOT(closeClicked()));
        connect(sensor->options, SIGNAL(clicked(bool)), sensor, SLOT(optionClicked()));
        connect(sensor->shape, SIGNAL(clicked(bool)), sensor, SLOT(shapeClicked()));

        //Connecting our sensor to this class
        //connect(sensor, SIGNAL(closeSensorSignal(QString)), this, SLOT(closeSensor(QString)));
        connect(sensor, SIGNAL(maxDuration(int)), this, SLOT(setMaxDuration(int)));

        addWidget(sensor->name, currentIndex, 0);
        addWidget(sensor, currentIndex, 1);
        addWidget(sensor->event, currentIndex, 2);
        addWidget(sensor->close, currentIndex, 3);
        addWidget(sensor->options, currentIndex, 4);
        addWidget(sensor->shape, currentIndex, 5);

        currentIndex++;

        //remake the event ui
        addWidget(save, currentIndex, 0);
        sensors[name] = sensor;
    }
    else
    {
        LOG_CAMPBELL() << "show all";
        sensors[name]->showAll();
    }
}

void ScrubEventManager::saveData()
{
    emit saveEvents();
}


void ScrubEventManager::setSensorTime(double time)
{
    data.sensorTime = time;
    updatePixmaps();
}

void ScrubEventManager::setVideoDuration(double videoTime)
{
    data.videoTime = videoTime;
    updatePixmaps();
}

void ScrubEventManager::setMaxDuration(int duration)
{
    maxDuration = duration;
}

void ScrubEventManager::closeEvents()
{
    for (auto it = sensors.begin(); it != sensors.end(); ++it)
    {
        delete it->second;
    }
    sensors.clear();
}

void ScrubEventManager::updatePixmaps()
{
    for (auto& kv : sensors)
    {
        kv.second->updatePixmap();
    }
}

void ScrubEventManager::updates()
{
    for (auto& kv : sensors)
    {
        kv.second->update();
    }
}

void ScrubEventManager::updatePixmap(QString name)
{
    if (sensors.find(name) != sensors.end())
    {
        sensors.at(name)->updatePixmap();
    }
}

void ScrubEventManager::eventUpdated(QString index)
{
    //make sure the event exists
    if (container->getEvents(index) != NULL)
    {
        addSensor(index);
    }
}

static int init = 2;
SensorBar::SensorBar(SharedData* data,  Events* event, QWidget *parent) : QWidget(parent)
{
    this->data = data;
    shapeType = init;
    init++;
    this->events = event;
}


void SensorBar::updatePixmap()
{
    setMinimumHeight(fontMetrics().height() + data->selectionSize);

    const int ratio = devicePixelRatio();
    const int l_width = width() * ratio;
    const int l_height = height() * ratio;
    const int l_margin = (data->margin) * ratio;

    pixmap = QPixmap(l_width, l_height);
    pixmap.fill(palette().window().color());

    QPainter p(&pixmap);
    p.setFont(font());
    QBrush brush(palette().text().color(), Qt::SolidPattern);
    QPen pen(brush, 1);
    p.setPen(pen);
    p.setBrush(brush);

    int newWidth = l_width - 2 * l_margin;


    emit maxDuration( data->sensorTime * data->fps);

    p.fillRect(l_margin, 0, newWidth, l_height, background);

    for (auto i = events->getIterator(); i != events->getEnd(); ++i)
    {

        if (shapeType >= 3)
        {

            int radius = data->selectionSize /2;
            int yPoint = data->selectionSize;
            int xPoint = *i / data->sensorTime * l_width;

            std::vector<QPoint> points;

            QPoint point;
            int angleIncrement = 360/shapeType;
            int angle = 0;

            float pi = 3.14159265;
            //generate points
            for (int k = 0; k < shapeType; k++)
            {
                point.setX(xPoint + (radius * cos(angle * pi/180)));
                point.setY(yPoint + (radius * sin(angle * pi/180)));
                points.push_back(point);
                angle += angleIncrement;
            }
            p.drawPolygon(&points.at(0), points.size());
        }
        else
        {
            int x1 = *i / data->sensorTime * l_width;
            QPoint center = QPoint(x1, height()/2);
            p.drawEllipse(center, height()/4, height()/4);
        }
    }

    p.end();
    update();
}

void SensorBar::paintEvent(QPaintEvent *e)
{

    const int ratio = devicePixelRatio();
    const int l_width = width() * ratio;  //* (data->sensorTime / data->videoTime);
    const int l_height = height() * ratio;

    QPainter p(this);
    QRect r = e->rect();
    p.setClipRect(r);
    p.drawPixmap(0, 0, l_width, l_height, pixmap);


    float x = (((float)data->time / (float)data->fps) / data->sensorTime) * l_width;
    p.drawArc(x, l_height/2, 5, 5, 0, 360 * 16);

    QBrush brush(palette().text().color(), Qt::SolidPattern);
    QPen pen(brush, 2);
    p.setPen(pen);
    p.setBrush(brush);



    if (!isEnabled())
    {
        LOG_CAMPBELL() << "not enabled";
        return;
    }
}



void SensorBar::closeClicked()
{
    //We would ask if they wanted to save it?

    name->hide();
    close->hide();
    event->hide();
    options->hide();
    shape->hide();
    hide();
    emit closeSensorSignal(name->text());
}

void SensorBar::showAll()
{
    name->show();
    close->show();
    event->show();
    options->show();
    shape->show();
    show();
}

void SensorBar::optionClicked()
{
    //Open option stuff
    QColorDialog dialog;
    background = dialog.getColor();
    updatePixmap();
}

void SensorBar::shapeClicked()
{
    QInputDialog dialog;
    QStringList list;
    list << "Triangle" << "Square" << "Pentagon" << "Hexagon" << "Heptagon" << "Circle";
    QString name = dialog.getItem(this, tr("Shape"), tr("Shapes"), list, 0, false);

    LOG_CAMPBELL() << name;

    if (name == tr("Triangle"))
    {
        shapeType = 3;
    }
    else if (name == tr("Square"))
    {
        shapeType = 4;
    }
    else if (name == tr("Pentagon"))
    {
        shapeType = 5;
    }
    else if (name == tr("Hexagon"))
    {
        shapeType = 6;
    }
    else if (name == tr("Heptagon"))
    {
        shapeType = 7;
    }
    else
    {
        shapeType = 0;
    }
    updatePixmap();
    LOG_CAMPBELL() << QString::number(shapeType);
}

SensorBar::~SensorBar()
{
    delete name;
    delete close;
    delete event;
    delete options;
    delete shape;
}
