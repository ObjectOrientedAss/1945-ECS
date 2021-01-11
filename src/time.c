#include "time.h"

struct Time *TimeInit()
{
    struct Time* t = calloc(1, sizeof(struct Time));
    t->_currentCount = SDL_GetPerformanceCounter();
    t->_lastCount = t->_currentCount;
    return t;
}

void TimeUpdate(struct Time *time)
{
    time->_lastCount = time->_currentCount;
    time->_currentCount = SDL_GetPerformanceCounter();
    time->_frequency = (double)SDL_GetPerformanceFrequency();
    time->deltaTime = (double)((time->_currentCount - time->_lastCount) * 1000 / time->_frequency);
    time->deltaTimeInSeconds = time->deltaTime * 0.001; //suka!
    time->fps = (int)(1.0 / time->deltaTime);
}