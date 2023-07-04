#include "Animator.h"

#include <string.h>

#include "Time.h"

Animator::Animator(SDL_Texture* spriteSheet, int spriteWidth, int spriteHeight, float frameTime, int *frameIndices, int frameIndicesCount) {
    this->spriteSheet = spriteSheet;
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;

    this->frameIndicesCount = frameIndicesCount;
    this->frameIndices = new int[frameIndicesCount];
    memcpy(this->frameIndices, frameIndices, frameIndicesCount * sizeof(int));

    this->frameTime = frameTime;
    currentFrameIndex = 0;
    frameTimeRemaining = frameTime;
}

Animator::~Animator() {
    delete[] frameIndices;
}

void Animator::update() {
    frameTimeRemaining -= Time::deltaTime;
    while (frameTimeRemaining <= 0) {
        currentFrameIndex = (currentFrameIndex + 1) % frameIndicesCount;
        frameTimeRemaining += frameTime;
    }
}

void Animator::draw(SDL_Renderer* renderer, SDL_Point* position, bool flipHorizontally) {
    SDL_Rect sourceRectangle = {
        frameIndices[currentFrameIndex] * spriteWidth,
        0,
        spriteWidth,
        spriteHeight
    };

    SDL_Rect destinationRectangle = {
        position->x,
        position->y,
        spriteWidth,
        spriteHeight
    };

    SDL_RenderCopyEx(
        renderer,
        spriteSheet,
        &sourceRectangle,
        &destinationRectangle,
        0,
        NULL,
        flipHorizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
