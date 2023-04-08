#include "Sensor.h"

Sensor::Sensor()
{
    this->isOn = true;  // heart monitor is on by default
}

/**
  update heart monitor sensor state
  alert MainWindow to change ui
  @param {int} new state
  @return {void} Returns nothing
*/
void Sensor::changeSensorState(bool state)
{
    qInfo() << "Heart monitor sensor is set to: " << state;
    this->isOn = state;
    emit sensorStateChanged();
}
