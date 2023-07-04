#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SDL.h>

class Animator {

public:

    Animator(SDL_Texture* spriteSheet, int spriteWidth, int spriteHeight, float frameTime, int *frameIndices, int frameIndicesCount);
    ~Animator();

    void reset();
    void update();
    void draw(SDL_Renderer* renderer, SDL_Point* position, bool flipHorizontally);

private:

    SDL_Texture *spriteSheet;
    int spriteWidth;
    int spriteHeight;

    int *frameIndices;
    int frameIndicesCount;
    float frameTime;

    int currentFrameIndex;
    float frameTimeRemaining;
};

#endif
