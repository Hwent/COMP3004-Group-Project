#include "Screen.h"

Screen::Screen()
{

}

/**
  returns Sensor instance
  (to be used for slot/signal in MainWindow to change ui)
  @return {Sensor*} Returns Sensor instance
*/
Sensor* Screen::getSensor()
{
    return &this->heartMonitor;
}

/**
  returns Battery instance
  (to be used for slot/signal in MainWindow to change ui)
  @return {Battery*} Returns Battery instance
*/
Battery* Screen::getBattery()
{
    return &this->batteryLevel;
}
