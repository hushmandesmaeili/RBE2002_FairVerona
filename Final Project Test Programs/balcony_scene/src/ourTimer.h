#pragma once

class ourTimer
{
public:
    ourTimer(unsigned long interval);
    bool isExpired();
    void reset();
    void reset(unsigned long newInterval);

private:
    unsigned long expiredTime;
    unsigned long timeInterval;
};
