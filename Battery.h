#ifndef BATTERY_H
#define BATTERY_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Battery: public QObject{

    Q_OBJECT;

private:
    qreal batteryLevel;

public:
    Battery();
    void updateBatteryLevel(qreal);
    qreal getBatteryLevel();

signals:
    void batteryLevelUpdated();     // send signal to change battery widget
};

#endif // BATTERY_H
