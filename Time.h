#ifndef TIME_H
#define TIME_H

#include <SDL.h>

class Time {

public:

    static void initialize(int maxFps);
    static void signalFrameStart();
    static void waitUntilFrameEnd();

private:

    static Uint64 frameMilliseconds;
    static Uint64 frameEndTimestamp;
};

#endif
