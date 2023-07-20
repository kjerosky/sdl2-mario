#include "Animator.h"

#include <string.h>

Animator::Animator(SDL_Texture* spriteSheet, int spriteWidth, int spriteHeight, int framesPerAnimationFrame, int *frameIndices, int frameIndicesCount) {
    this->spriteSheet = spriteSheet;
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;
    this->framesPerAnimationFrame = framesPerAnimationFrame;

    this->frameIndicesCount = frameIndicesCount;
    this->frameIndices = new int[frameIndicesCount];
    memcpy(this->frameIndices, frameIndices, frameIndicesCount * sizeof(int));

    reset();
}

Animator::~Animator() {
    delete[] frameIndices;
}

void Animator::reset() {
    currentFrameIndex = 0;
    framesRemaining = framesPerAnimationFrame;
}

bool Animator::update() {
    bool animationCompleted = false;

    framesRemaining--;
    if (framesRemaining <= 0) {
        currentFrameIndex++;
        if (currentFrameIndex >= frameIndicesCount) {
            animationCompleted = true;
            currentFrameIndex = 0;
        }
        framesRemaining = framesPerAnimationFrame;
    }

    // animation completed status
    return animationCompleted;
}

void Animator::draw(SDL_Renderer* renderer, SDL_Point* position, bool flipHorizontally, bool flipVertically) {
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

    int horizontalFlipping = flipHorizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int verticalFlipping = flipVertically ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
    SDL_RendererFlip flipping = (SDL_RendererFlip)(horizontalFlipping | verticalFlipping);

    SDL_RenderCopyEx(
        renderer,
        spriteSheet,
        &sourceRectangle,
        &destinationRectangle,
        0,
        NULL,
        flipping);
}
