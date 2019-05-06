#ifndef GRAPHDATACONNECTOR_H
#define GRAPHDATACONNECTOR_H

#include <QObject>
#include "analyzer/graphstats.h"

class GraphDataConnector : public QObject
{
    Q_OBJECT
public:
    static GraphDataConnector& getInstance();
    ~GraphDataConnector();

    QStringList getNames();
    std::map<float, float> getData(QString name);
    GraphStats getStats(QString name);

    //delete the methods we dont want
    GraphDataConnector(GraphDataConnector const&) = delete;
    void operator=(GraphDataConnector const&) = delete;

public slots:
    void addData(QString name, std::map<float, float> data, GraphStats stats);
    void removeData(QString name);

signals:
    void updateData();

private:
    explicit GraphDataConnector(QObject *parent = nullptr);


    struct Data
    {
        std::map<float, float> data;
        GraphStats stats;
    };

    //references to our data set
    std::map<QString, Data> information;
};

#endif // GRAPHDATACONNECTOR_H

