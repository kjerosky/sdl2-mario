#include "Time.h"

Uint64 Time::previousTimestamp;
float Time::deltaTime;

void Time::initialize() {
    previousTimestamp = SDL_GetTicks64();
}

void Time::pause() {
    previousTimestamp = SDL_GetTicks64();
    deltaTime = 0.0f;
}

void Time::update() {
    Uint64 currentTimestamp = SDL_GetTicks64();
    deltaTime = (currentTimestamp - previousTimestamp) / 1000.0f;

    previousTimestamp = currentTimestamp;
}
