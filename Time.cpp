#include "Time.h"

float Time::deltaTime;
Uint64 Time::frameMilliseconds;
Uint64 Time::frameEndTimestamp;

void Time::initialize(int maxFps) {
    frameMilliseconds = (1.0f / maxFps) * 1000;
    deltaTime = 1.0f / maxFps;
}

void Time::signalFrameStart() {
    frameEndTimestamp = SDL_GetTicks64() + frameMilliseconds;
}

void Time::waitUntilFrameEnd() {
    while (SDL_GetTicks64() <= frameEndTimestamp)
        ;
}
