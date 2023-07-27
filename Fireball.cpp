#include "Fireball.h"

#include <iostream>

#include "SpriteSheetRepository.h"

const float Fireball::HORIZONTAL_VELOCITY = 4.0f;
const float Fireball::MAX_VERTICAL_VELOCITY = 4.0f;
const float Fireball::GRAVITY = 0.5f;
const int Fireball::MOVING_SPRITE_WIDTH = 8;

Fireball::Fireball(Level *currentLevel, SDL_FPoint *position, GameObjectsManager *objectsManager, bool facingRight) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsManager = objectsManager;
    this->facingRight = facingRight;

    collisionSystem = CollisionSystem::getInstance();

    velocity.x = facingRight ? HORIZONTAL_VELOCITY : -HORIZONTAL_VELOCITY;
    velocity.y = MAX_VERTICAL_VELOCITY;

    spriteSheet = SpriteSheetRepository::getInstance()->getSpriteSheetById(FIREBALL_SHEET);

    int movingFrames[] = {0, 1, 2, 3};
    int movingFramesCount = sizeof(movingFrames) / sizeof(int);
    movingAnimator = new Animator(spriteSheet, 4, movingFrames, movingFramesCount);

    int explodingFrames[] = {4, 5, 6};
    int explodingFramesCount = sizeof(explodingFrames) / sizeof(int);
    explodingAnimator = new Animator(spriteSheet, 2, explodingFrames, explodingFramesCount);

    currentAnimator = movingAnimator;

    hitbox.x = 4;
    hitbox.y = 0;
    hitbox.w = 8;
    hitbox.h = 8;

    collisionPoint.x = facingRight ? 11 : 4;
    collisionPoint.y = 7;

    exploding = false;
}

Fireball::~Fireball() {
    delete movingAnimator;
    delete explodingAnimator;
}

GameObjectType Fireball::getType() {
    return FIREBALL;
}

SDL_Rect* Fireball::getHitBox() {
    return &hitbox;
}

bool Fireball::isCollidable() {
    return !exploding;
}

bool Fireball::isStompable() {
    return false;
}

bool Fireball::isDrawnOnTop() {
    return true;
}

CollisionResponse Fireball::receiveCollision(GameObject* sourceObject) {
    CollisionResponse response = {NO_PROBLEM, this};
    switch (sourceObject->getType()) {
        case ENEMY: {
            beginExploding();
            response.type = TAKE_DAMAGE;
        } break;

        default: {
            response.type = NO_PROBLEM;
        }
    }

    return response;
}

void Fireball::update(SDL_Point *cameraPosition) {
    bool previouslyExploding = exploding;
    if (!exploding) {
        applyVerticalMovement();
    }
    if (!exploding) {
        applyHorizontalMovement();
    }
    if (!exploding) {
        resolveCollisions();
    }

    if (previouslyExploding != exploding) {
        beginExploding();
    }

    bool animationComplete = currentAnimator->update();
    if (exploding && animationComplete) {
        objectsManager->destroy(this);
    }
}

void Fireball::applyHorizontalMovement() {
    position.x += velocity.x;
    SDL_Point testPoint = {
        (int)position.x + collisionPoint.x,
        (int)position.y + collisionPoint.y,
    };
    if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
        exploding = true;
    }
}

void Fireball::applyVerticalMovement() {
    float previousPositionY = position.y;

    velocity.y = SDL_min(velocity.y + GRAVITY, MAX_VERTICAL_VELOCITY);
    position.y += velocity.y;

    SDL_Point testPoint = {
        (int)position.x + collisionPoint.x,
        (int)position.y + collisionPoint.y,
    };
    if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
        position.y = previousPositionY;
        velocity.y = -MAX_VERTICAL_VELOCITY;
    }
}

void Fireball::resolveCollisions() {
    std::vector<CollisionResponse>* collisionResponses = collisionSystem->testObjectAgainstAllOthers(this);
    for (std::vector<CollisionResponse>::iterator responseIterator = collisionResponses->begin(); responseIterator != collisionResponses->end(); responseIterator++) {
        if ((*responseIterator).type == TAKE_DAMAGE) {
            beginExploding();
        }
    }
}

void Fireball::beginExploding() {
    exploding = true;
    position.y -= 4;
    currentAnimator = explodingAnimator;
}

void Fireball::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    currentAnimator->draw(renderer, &spritePosition, !facingRight, false);
}
