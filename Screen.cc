#include "Screen.h"

Screen::Screen()
{
    this->menuOptions.push_back("Start/End Session");
    this->menuOptions.push_back("Settings");
    this->menuOptions.push_back("Log/History");
    //
    this->log.push_back(SimulationData(0,0,0));
    this->log.push_back(SimulationData(1,1,1));
}

/**
  returns Sensor instance
  (to be used for slot/signal in MainWindow to change ui)
  @return {Sensor*} Returns Sensor instance
*/
Sensor* Screen::getSensor(){return &this->heartMonitor;}

/**
  returns Battery instance
  (to be used for slot/signal in MainWindow to change ui)
  @return {Battery*} Returns Battery instance
*/
Battery* Screen::getBattery(){return &this->batteryLevel;}

vector<string> Screen::getMenuOptions(){ return this->menuOptions;}

vector<SimulationData> Screen::getlog(){ return this->log;}
