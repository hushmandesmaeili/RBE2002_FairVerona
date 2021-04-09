#include "ourTimer.h"
#include <Arduino.h>

/**
 * Create a timer that will expire every "interval"
 **/
ourTimer::ourTimer(unsigned long interval)
{
    expiredTime = millis() + interval;
    timeInterval = interval;
}

/**
 * Reset the timer to that the expired time is the current time + interval
 */
void ourTimer::reset()
{
    expiredTime = millis() + timeInterval;
}

/**
 * Change the timer interval to "NewInterval" then
 * reset the timer to that the expired time is the current time + interval
 */
void ourTimer::reset(unsigned long newInterval)
{
    timeInterval = newInterval;
    reset();
}

/**
 * Check if the timer is expired, that is the current time is past
 * the expired time.
 */
bool ourTimer::isExpired()
{
    unsigned long currentTime = millis();
    if (currentTime > expiredTime)
    {
        expiredTime = currentTime + timeInterval;
        return true;
    }
    return false;
}
