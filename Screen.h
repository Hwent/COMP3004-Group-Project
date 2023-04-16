#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
#include <QDebug>
#include <QMap>
#include <QDate>
#include "Sensor.h"
#include "Battery.h"
#include "SimulationData.h"

using namespace std;

class Screen: public QObject
{
    Q_OBJECT;

private:
    Sensor heartMonitor;
    Battery batteryLevel;
    vector<string> menuOptions;
    QMap<QDateTime, QVector<double>> log;
    SimulationData simulationData;


public:
    Screen();
    Sensor* getSensor();
    Battery* getBattery();
    vector<string> getMenuOptions();
    QMap<QDateTime, QVector<double>> getLog();
    void addLog(double, double, double);
};

#endif // SCREEN_H
