#ifndef TIME_H
#define TIME_H

#include "SDL.h"

struct Time
{
    double _frequency;
    int _currentCount;
    int _lastCount;
    double deltaTime;
    double deltaTimeInSeconds;
    int fps;
};

struct Time* TimeInit();
void TimeUpdate(struct Time*);

#endif //TIME_H