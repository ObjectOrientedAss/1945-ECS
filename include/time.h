#ifndef TIME_H
#define TIME_H

#include "SDL.h"

typedef struct
{
    double _frequency;
    Uint64 _currentCount;
    Uint64 _lastCount;
    float deltaTime;
    int fps;
} Time;

Time* TimeInit();
void TimeUpdate(Time*);

#endif //TIME_H