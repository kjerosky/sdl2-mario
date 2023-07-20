#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SDL.h>

class Animator {

public:

    Animator(SDL_Texture* spriteSheet, int spriteWidth, int spriteHeight, int framesPerAnimationFrame, int *frameIndices, int frameIndicesCount);
    ~Animator();

    void reset();
    bool update();
    void draw(SDL_Renderer* renderer, SDL_Point* position, bool flipHorizontally, bool flipVertically);

private:

    SDL_Texture *spriteSheet;
    int spriteWidth;
    int spriteHeight;

    int *frameIndices;
    int frameIndicesCount;
    int framesPerAnimationFrame;

    int framesRemaining;
    int currentFrameIndex;
};

#endif
