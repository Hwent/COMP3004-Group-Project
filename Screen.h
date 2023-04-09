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

public:
    Screen();
    Sensor* getSensor();
    Battery* getBattery();
};

#endif // SCREEN_H
