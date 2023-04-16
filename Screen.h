#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
#include <QDebug>
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
    //vector<SimulationData> log;



public:
    Screen();
    Sensor* getSensor();
    Battery* getBattery();
    vector<string> getMenuOptions();
    //vector<SimulationData> getlog();
};

#endif // SCREEN_H
