#include "Device.h"

Device::Device()
{

}

/**
  returns Sensor instance from Screen
  (to be used for slot/signal in MainWindow to change ui)
  @return {Sensor*} Returns Sensor instance
*/
Sensor* Device::getSensor()
{
    return this->screen.getSensor();
}
