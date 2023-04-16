#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <string>
#include <QDebug>
#include "Screen.h"
#include "history.h"

using namespace std;

class Device: public QObject
{
    Q_OBJECT;

private:
    Screen screen;
    History history;

public:
    Device();
    Sensor* getSensor();
    Battery* getBattery();
    Screen* getScreen();
    History* getHistory();
};

#endif // DEVICE_H
