#include "BreathPaceWidget.h"

// Constructor
BreathPaceWidget::BreathPaceWidget()
{
    this->interval = 10;
}

/**
  update breath pacer
  @param {int} new breath pace
  @return {void} Returns nothing
*/
void BreathPaceWidget::updateInterval(int newBreathPaceInterval)
{
    this->interval = newBreathPaceInterval;

    emit intervalUpdated(newBreathPaceInterval);
}
