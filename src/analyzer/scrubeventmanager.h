#ifndef EVENTBAR_H
#define EVENTBAR_H

#include <QWidget>
#include <map>
#include <QGridLayout>
#include <qlabel>
#include <QPushButton>
#include <events.h>

struct SharedData
{
    int time;
    int size;
    int margin;
    int selectionSize;
    double videoTime;
    double sensorTime;
    int fps;
};

class SensorBar : public QWidget
{
    Q_OBJECT
public:
    explicit SensorBar(SharedData* info, Events* event, QWidget *parent = nullptr);
    ~SensorBar();
    void updatePixmap();
    virtual void paintEvent(QPaintEvent *event);
    void showAll();

    QLabel* name;
    QPushButton* close;
    QPushButton* event;
    QPushButton* options;
    QPushButton* shape;


    void setVideoTime(double);
    void setSensorTime(double);


public slots:
    //void addEvent();
    void closeClicked();

private slots:
    void optionClicked();
    void shapeClicked();

signals:
    void closeSensorSignal(QString);
    void maxDuration(int);
    void eventClicked();

private:

    //kinda bad since it points to a variable in another class
    SharedData *data;

    Events* events;

    int shapeType;
    QPixmap pixmap;
    QColor background;
};



class ScrubEventManager : public QGridLayout
{
    Q_OBJECT
public:
    explicit ScrubEventManager(EventContainer* container, QWidget *parent = nullptr);

    void addEventToBar(QString);
    void addSensor(QString);

    void setMargin(int margin);
    void setSelectionSize(int size);
    void setFPS(int fps);

protected:

private:

    SharedData data;

    std::map<QString, SensorBar*> sensors;
    EventContainer* container;

    int currentIndex;
    QPushButton* save;

    double maxDuration;
    int millaSecondConversion;

    void updatePixmaps();
    void updatePixmap(QString);
    void updates();

signals:
    //save the file
    void saveEvents();

public slots:

    //updates our display
    void eventUpdated(QString);

    void setMaxDuration(int);

    void setSensorTime(double time);
    void setVideoDuration(double);

    void setTime(int position);
    void saveData();

    void closeEvents();
};



#endif // EVENTBAR_H
