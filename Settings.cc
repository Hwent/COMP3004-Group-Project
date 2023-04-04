#include "Settings.h"

// Constructor
Settings::Settings()
{
    this->challengeLevel = 1;
    this->breathPacer = 10;
}

/**
  update challenge level
  @param {int} new challenge level
  @return {void} Returns nothing
*/

void Settings::updateChallengLevel(int challengeLevel)
{
    this->challengeLevel = challengeLevel;
}

/**
  update breath pacer
  @param {int} new breath pacer setting
  @return {void} Returns nothing
*/

void Settings::updateBreathPacer(int breathPacer)
{
    this->breathPacer = breathPacer;
}

/**

  @param {int} new breath pacer interval
  @return {void} Returns nothing
*/
void Settings::handleUpdatedInterval(int interval)
{
    this->breathPacer = interval;
}
