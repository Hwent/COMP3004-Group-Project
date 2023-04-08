#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>
#include <QDebug>
#include "Sensor.h"

using namespace std;

class Screen: public QObject
{
    Q_OBJECT;

private:
    Sensor heartMonitor;

public:
    Screen();
    Sensor* getSensor();
};

#endif // SCREEN_H
