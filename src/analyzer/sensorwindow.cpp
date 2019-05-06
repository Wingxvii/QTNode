#include "sensorwindow.h"
#include "ui_sensorwindow.h"
#include <QLabel>
#include <Logger.h>
#include <QResizeEvent>

SensorWindow::SensorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SensorWindow)
{
    ui->setupUi(this);
    ui->titles->addWidget(new QLabel("testing"));

    m_sensorManager = new SensorManager(this);

    //setup
    ui->options->setSizeConstraint(QLayout::SetFixedSize);
    m_sensorManager->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    ui->options->addWidget(m_sensorManager, 0);
    m_sensorManager->adjustSize();

    m_graphs = new GraphController(m_sensorManager->getContainer(), this, ui->graphs);

    connect(this, SIGNAL(redraw()), m_graphs, SLOT(updateGraphs()));
    connect(m_sensorManager, SIGNAL(createGraphDoc(QString,QJsonArray)), m_graphs, SLOT(setupData(QString, QJsonArray)));
    connect(m_sensorManager, SIGNAL(graphOpened(QString)), m_graphs, SLOT(setDocked(QString)));
    connect(m_sensorManager, SIGNAL(clear()), m_graphs, SLOT(ClearData()));
    connect(m_sensorManager, SIGNAL(createEvents(QString)), m_graphs, SLOT(createEvents(QString)));
    connect(m_sensorManager, SIGNAL(createVirtual(QString)), m_graphs, SLOT(createVirtual(QString)));

    refreshSize = 200;
}

void SensorWindow::resizeEvent(QResizeEvent *e)
{
    LOG_CAMPBELL() << "resizing";


    if (std::abs(e->size().width() - currentSize.width()) > refreshSize
            || std::abs(e->size().height() - currentSize.height()) > refreshSize )
    {
        QSize temp(std::abs(e->size().width() - currentSize.width()),
                   std::abs(e->size().height() - currentSize.height()));

        LOG_CAMPBELL() << QString::number(temp.height());
        LOG_CAMPBELL() << QString::number(temp.width());

        currentSize = e->size();
        emit redraw();
    }
}

SensorManager* SensorWindow::getManager()
{
    return m_sensorManager;
}
