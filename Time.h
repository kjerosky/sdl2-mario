#ifndef TIME_H
#define TIME_H

#include <SDL.h>

class Time {

public:

    static void initialize();
    static void pause();
    static void update();

    static float deltaTime;

private:

    static Uint64 previousTimestamp;
};

#endif
