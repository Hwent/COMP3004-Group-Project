#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <QDebug>
#include "Screen.h"

using namespace std;

class Device: public QObject
{
    Q_OBJECT;

private:
    Screen screen;

public:
    Device();
    Sensor* getSensor();
    Battery* getBattery();
};

#endif // DEVICE_H
