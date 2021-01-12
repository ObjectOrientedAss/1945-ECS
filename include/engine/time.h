#ifndef TIME_H
#define TIME_H

#include "SDL.h"

//basic structure containing all the informations as deltatime, deltatime in seconds, and fps
struct Time
{
    double _frequency;
    int _currentCount;
    int _lastCount;
    double deltaTime;           //time in milliseconds since the last frame tick
    double deltaTimeInSeconds;  //time in seconds since the last frame tick
    int fps;
};

//allocate and start the time ticking
struct Time* __TimeInit();
//update the time data (once per frame)
void __TimeUpdate(struct Time*);

#endif //TIME_H