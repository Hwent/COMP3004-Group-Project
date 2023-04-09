#include "Battery.h"

Battery::Battery()
{
    this->batteryLevel = 100;   // default battery setting
}

/**
  update battery level, and emit signal to MainWindow
  @param {qreal} new battery level
  @return {void} Returns nothing
*/
void Battery::updateBatteryLevel(qreal newBattery)
{
    qInfo() << "Battery level is set to: " << newBattery;
    this->batteryLevel = newBattery;
    emit batteryLevelUpdated();

}

qreal Battery::getBatteryLevel()
{
    return this->batteryLevel;
}
