#ifndef SIMULATIONDATA_H
#define SIMULATIONDATA_H

#include <iostream>
#include <string>
#include <QDebug>
#include <QMap>
#include <QDateTime>

using namespace std;

class SimulationData {

private:

    QMap<QDateTime, QVector<double>> simulationLog; // date: [coherence, length, achievement]

public:
    SimulationData();
//    QString toString();
    void addLog(double, double, double);
    QMap<QDateTime, QVector<double>> getLog();

};

#endif // SIMULATIONDATA_H
