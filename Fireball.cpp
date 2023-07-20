#include "Fireball.h"

#include <SDL_image.h>
#include <iostream>

const float Fireball::HORIZONTAL_VELOCITY = 4.0f;
const float Fireball::MAX_VERTICAL_VELOCITY = 4.0f;
const int Fireball::MOVING_SPRITE_WIDTH = 8;

Fireball::Fireball(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, GameObjectsManager *objectsManager, bool facingRight) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsManager = objectsManager;
    objectsList = objectsManager->getObjectList();
    this->facingRight = facingRight;

    velocity.x = facingRight ? HORIZONTAL_VELOCITY : -HORIZONTAL_VELOCITY;
    velocity.y = MAX_VERTICAL_VELOCITY;

    spriteSheet = IMG_LoadTexture(renderer, "assets/fireball.png");
    if (!spriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    int movingFrames[] = {0, 1, 2, 3};
    int movingFramesCount = sizeof(movingFrames) / sizeof(int);
    movingAnimator = new Animator(spriteSheet, 16, 16, 0.067, movingFrames, movingFramesCount);

    int explodingFrames[] = {4, 5, 6};
    int explodingFramesCount = sizeof(explodingFrames) / sizeof(int);
    explodingAnimator = new Animator(spriteSheet, 16, 16, 0.067, explodingFrames, explodingFramesCount);

    currentAnimator = movingAnimator;

    hitbox.x = 4;
    hitbox.y = 0;
    hitbox.w = 8;
    hitbox.h = 8;

    collisionPointsCount = 2;
    collisionPoints = new SDL_Point[collisionPointsCount];
    collisionPoints[0].x = facingRight ? 11 : 4;
    collisionPoints[0].y = 0;
    collisionPoints[1].x = facingRight ? 11 : 4;
    collisionPoints[1].y = 7;

    exploding = false;
}

Fireball::~Fireball() {
    SDL_DestroyTexture(spriteSheet);

    delete movingAnimator;
    delete explodingAnimator;

    delete[] collisionPoints;
}

GameObject::Type Fireball::getType() {
    return FIREBALL;
}

SDL_Rect* Fireball::getHitBox() {
    return &hitbox;
}

bool Fireball::isStompable() {
    return false;
}

GameObject::CollisionResponse Fireball::receiveCollision(GameObject* sourceObject) {
    //TODO
    return NO_PROBLEM;
}

void Fireball::update(SDL_Point *cameraPosition) {
    bool previouslyExploding = exploding;
    if (!exploding) {
        applyHorizontalMovement();
    }
    // if (!exploding) {
    //     applyVerticalMovement();
    // }

    if (previouslyExploding != exploding) {
        position.y -= 4;
        currentAnimator = explodingAnimator;
    } else if (exploding) {
        //TODO
    }

    currentAnimator->update();
}

void Fireball::applyHorizontalMovement() {
    position.x += velocity.x;
    for (int i = 0; i < collisionPointsCount; i++) {
        SDL_Point collisionPoint = collisionPoints[i];
        SDL_Point testPoint = {
            (int)position.x + collisionPoint.x,
            (int)position.y + collisionPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            exploding = true;
        }
    }
}

void Fireball::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y
    };

    currentAnimator->draw(renderer, &spritePosition, !facingRight);
}
