#include "Goomba.h"

#include <iostream>
#include <SDL_image.h>

Goomba::Goomba(SDL_Renderer* renderer, Level* currentLevel, SDL_FPoint* position) {
    this->position = *position;
    this->currentLevel = currentLevel;

    velocity.x = -0.5f; // fake velocity
    velocity.y = 0;

    downCollisionChecksCount = 2;
    downCollisionChecks = new SDL_Point[downCollisionChecksCount];
    downCollisionChecks[0].x = 0;
    downCollisionChecks[0].y = 15;
    downCollisionChecks[1].x = 15;
    downCollisionChecks[1].y = 15;

    leftCollisionChecksCount = 2;
    leftCollisionChecks = new SDL_Point[leftCollisionChecksCount];
    leftCollisionChecks[0].x = 0;
    leftCollisionChecks[0].y = 0;
    leftCollisionChecks[1].x = 0;
    leftCollisionChecks[1].y = 15;

    spriteSheet = IMG_LoadTexture(renderer, "assets/goomba.png");
    if (!spriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    int walkingFrames[] = {0, 1};
    int walkingFramesCount = sizeof(walkingFrames) / sizeof(int);
    walkingAnimator = new Animator(spriteSheet, 16, 16, 0.15f, walkingFrames, walkingFramesCount);

    int dyingFrames[] = {2};
    int dyingFramesCount = sizeof(dyingFrames) / sizeof(int);
    dyingAnimator = new Animator(spriteSheet, 16, 16, 1.0f, dyingFrames, dyingFramesCount);

    currentAnimator = walkingAnimator;
}

Goomba::~Goomba() {
    SDL_DestroyTexture(spriteSheet);

    delete walkingAnimator;
    delete dyingAnimator;

    delete[] downCollisionChecks;
    delete[] leftCollisionChecks;
}

void Goomba::update(SDL_Point *cameraPosition) {
    Animator* previousAnimator = currentAnimator;

    //TODO THIS IS TESTING ONLY
    position.x += velocity.x;
    for (int i = 0; i < leftCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = leftCollisionChecks[i];
        if (velocity.x > 0) {
            collisionCheckPoint.x = 15 - collisionCheckPoint.x;
        }

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            position.x = (int)position.x;

            testPoint.y = (int)position.y + collisionCheckPoint.y;
            do {
                position.x += velocity.x < 0 ? 1 : -1;
                testPoint.x = (int)position.x + collisionCheckPoint.x;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));

            velocity.x = -velocity.x;
            break;
        }
    }

    //TODO THIS IS TESTING ONLY
    velocity.y += 0.1; // fake gravity effect
    position.y += velocity.y;
    for (int i = 0; i < downCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = downCollisionChecks[i];

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.y = 0;
            position.y = (int)position.y;

            testPoint.x = (int)position.x + collisionCheckPoint.x;
            do {
                position.y--;
                testPoint.y = (int)position.y + collisionCheckPoint.y;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }

    if (previousAnimator != currentAnimator) {
        currentAnimator->reset();
    } else {
        currentAnimator->update();
    }
}

void Goomba::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y
    };

    currentAnimator->draw(renderer, &spritePosition, false);
}
