#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <QDebug>
#include "Screen.h"
#include "Button.h"

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
    Screen* getScreen();
};

#endif // DEVICE_H
