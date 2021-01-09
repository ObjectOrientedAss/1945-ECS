#ifndef TIME_H
#define TIME_H

#include "SDL.h"

typedef struct
{
    double _frequency;
    int _currentCount;
    int _lastCount;
    double deltaTime;
    double deltaTimeInSeconds;
    int fps;
} Time;

Time* TimeInit();
void TimeUpdate(Time*);

#endif //TIME_H