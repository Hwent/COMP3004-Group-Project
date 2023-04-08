#include "Battery.h"

Battery::Battery()
{
    this->batteryLevel = 100;   // default battery setting
}

/**
  update battery level
  @param {int} new battery level
  @return {void} Returns nothing
*/
void Battery::updateBatteryLevel(qreal newBattery)
{
    this->batteryLevel = newBattery;
}
