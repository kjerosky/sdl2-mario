#include "Animator.h"

#include <string.h>

Animator::Animator(SpriteSheet *spriteSheet, int framesPerAnimationFrame, int *frameIndices, int frameIndicesCount) {
    this->spriteSheet = spriteSheet;
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

void Animator::draw(SDL_Renderer *renderer, SDL_Point *position, bool flipHorizontally, bool flipVertically) {
    spriteSheet->drawSprite(renderer, frameIndices[currentFrameIndex], position, flipHorizontally, flipVertically);
}
