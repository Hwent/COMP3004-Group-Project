#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>
#include <QDebug>

using namespace std;

class Sensor : public QObject
{
    Q_OBJECT;

private:
    bool isOn;

public:
    Sensor();
    void changeSensorState(bool);   // change to false if heart monitor turns off

signals:
    void sensorStateChanged();  // when sensor state is changed, alert ui
};

#endif // SENSOR_H
