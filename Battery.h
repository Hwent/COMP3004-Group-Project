#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Battery {
private:
    qreal batteryLevel;

public:
    Battery();
    void updateBatteryLevel(qreal);
};

#endif // BATTERY_H
