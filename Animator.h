#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SDL.h>

#include "SpriteSheet.h"

class Animator {

public:

    Animator(SpriteSheet* TESTspriteSheet, int framesPerAnimationFrame, int *frameIndices, int frameIndicesCount);
    ~Animator();

    void reset();
    bool update();
    void draw(SDL_Renderer* renderer, SDL_Point* position, bool flipHorizontally, bool flipVertically);

private:

    SpriteSheet* spriteSheet;

    int *frameIndices;
    int frameIndicesCount;
    int framesPerAnimationFrame;

    int framesRemaining;
    int currentFrameIndex;
};

#endif
