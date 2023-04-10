#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
#include <QDebug>
#include "Sensor.h"
#include "Battery.h"

using namespace std;

class Screen: public QObject
{
    Q_OBJECT;

private:
    Sensor heartMonitor;
    Battery batteryLevel;
    vector<string> menuOptions;

public:
    Screen();
    Sensor* getSensor();
    Battery* getBattery();
    vector<string> getMenuOptions();
};

#endif // SCREEN_H
