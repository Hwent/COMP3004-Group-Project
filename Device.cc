#include "Device.h"

Device::Device()
{

}

/**
  returns Sensor instance from Screen
  (to be used for slot/signal in MainWindow to change ui)
  @return {Sensor*} Returns Sensor instance
*/
Sensor* Device::getSensor(){return this->screen.getSensor();}

/**
  returns Battery instance from Screen
  (to be used for slot/signal in MainWindow to change ui)
  @return {Battery*} Returns Battery instance
*/
Battery* Device::getBattery(){return this->screen.getBattery();}

Screen* Device::getScreen() {return &this->screen; }
