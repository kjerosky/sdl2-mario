#include "Goomba.h"

#include <iostream>

#include "Time.h"
#include "GameConfig.h"
#include "SpriteSheetRepository.h"

const float Goomba::HORIZONTAL_VELOCITY = -0.5f;
const float Goomba::GRAVITY = 0.1f;
const Uint64 Goomba::STOMPED_FRAMES = 30;
const float TAKING_DAMAGE_INITIAL_HORIZONTAL_VELOCITY = 1.0f;
const float TAKING_DAMAGE_INITIAL_VERTICAL_VELOCITY = -2.5f;

Goomba::Goomba(Level* currentLevel, SDL_FPoint* position, GameObjectsManager* objectsManager) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsManager = objectsManager;

    collisionSystem = CollisionSystem::getInstance();

    velocity.x = HORIZONTAL_VELOCITY;
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

    hitBox.x = 3;
    hitBox.y = 6;
    hitBox.w = 10;
    hitBox.h = 10;

    spriteSheet = SpriteSheetRepository::getInstance()->getSpriteSheetById(GOOMBA_SHEET);

    int walkingFrames[] = {0, 1};
    int walkingFramesCount = sizeof(walkingFrames) / sizeof(int);
    walkingAnimator = new Animator(spriteSheet, 8, walkingFrames, walkingFramesCount);

    int stompedFrames[] = {2};
    int stompedFramesCount = sizeof(stompedFrames) / sizeof(int);
    stompedAnimator = new Animator(spriteSheet, STOMPED_FRAMES, stompedFrames, stompedFramesCount);

    int hitByFireballFrames[] = {0};
    int hitByFireballFramesCount = sizeof(hitByFireballFrames) / sizeof(int);
    hitByFireballAnimator = new Animator(spriteSheet, 1, hitByFireballFrames, hitByFireballFramesCount);

    currentAnimator = walkingAnimator;

    state = WALKING;
}

Goomba::~Goomba() {
    delete walkingAnimator;
    delete stompedAnimator;

    delete[] downCollisionChecks;
    delete[] leftCollisionChecks;
}

GameObjectType Goomba::getType() {
    return ENEMY;
}

SDL_Rect* Goomba::getHitBox() {
    return &hitBox;
}

bool Goomba::isCollidable() {
    return state == WALKING;
}

bool Goomba::isStompable() {
    return true;
}

bool Goomba::isDrawnOnTop() {
    return state == HIT_BY_FIREBALL;
}

CollisionResponse Goomba::receiveCollision(GameObject* sourceObject) {
    CollisionResponse response = {NO_PROBLEM, this};
    GameObjectType sourceType = sourceObject->getType();
    switch (sourceType) {
        case PLAYER: {
            bool isPlayerFalling = sourceObject->getVelocity()->y > 0;
            if (isPlayerFalling) {
                response.type = REACT_TO_STOMP;
                state = STOMPED;
            } else {
                response.type = TAKE_DAMAGE;
            }
        } break;

        case ENEMY: {
            velocity.x = -velocity.x;
            response.type = REVERSE_COURSE;
        } break;

        case FIREBALL: {
            takeDamage(sourceObject->getVelocity()->x >= 0);
            response.type = TAKE_DAMAGE;
        } break;

        case BLOCK_BUMP: {
            takeDamage(velocity.x >= 0);
            response.type = NO_PROBLEM;
        } break;

        default: {
            response.type = NO_PROBLEM;
        } break;
    }

    return response;
}

void Goomba::update(SDL_Point *cameraPosition) {
    checkStateTransitions();
    processCurrentState();
    animateSprite();
}

void Goomba::checkStateTransitions() {
    switch (state) {
        case WALKING:
            // nothing to do - transition is handled through collisions
            break;

        case STOMPED:
            // nothing to do - transition is set by the animator
            break;

        case HIT_BY_FIREBALL:
            // nothing to do - will transition out when offscreen
            break;

        case DEAD:
            // nothing to do
            break;
    }
}

void Goomba::processCurrentState() {
    switch (state) {
        case WALKING:
            applyHorizontalMovement();
            applyVerticalMovement();
            resolveCollisions();
            break;

        case STOMPED:
            applyVerticalMovement();
            break;

        case HIT_BY_FIREBALL:
            applyHitByFireballMovement();
            break;

        case DEAD:
            objectsManager->destroy(this);
            break;
    }
}

void Goomba::applyHorizontalMovement() {
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
}

void Goomba::applyVerticalMovement() {
    velocity.y += GRAVITY;
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
}

void Goomba::resolveCollisions() {
    bool shouldReverseCourse = false;
    bool shouldGetStomped = false;
    bool shouldTakeDamage = false;
    bool fallRightFromDamage = false;

    std::vector<CollisionResponse>* collisionResponses = collisionSystem->testObjectAgainstAllOthers(this);
    for (std::vector<CollisionResponse>::iterator responseIterator = collisionResponses->begin(); responseIterator != collisionResponses->end(); responseIterator++) {
        CollisionResponse response = *responseIterator;
        if (response.type == REVERSE_COURSE) {
            shouldReverseCourse = true;
        } else if (response.type == GET_STOMPED) {
            shouldGetStomped = true;
        } else if (response.type == TAKE_DAMAGE) {
            shouldTakeDamage = true;
            fallRightFromDamage = response.source->getVelocity()->x >= 0;
        }
    }

    if (shouldTakeDamage) {
        takeDamage(fallRightFromDamage);
    } else if (shouldGetStomped) {
        state = STOMPED;
    } else if (shouldReverseCourse) {
        velocity.x = -velocity.x;
    }
}

void Goomba::animateSprite() {
    Animator* previousAnimator = currentAnimator;

    if (state == STOMPED) {
        currentAnimator = stompedAnimator;
    } else if (state == HIT_BY_FIREBALL) {
        currentAnimator = hitByFireballAnimator;
    }

    if (previousAnimator != currentAnimator) {
        currentAnimator->reset();
    } else {
        bool animationComplete = currentAnimator->update();
        if (animationComplete && currentAnimator == stompedAnimator) {
            state = DEAD;
        }
    }
}

void Goomba::takeDamage(bool fallRight) {
    velocity.x = fallRight ? TAKING_DAMAGE_INITIAL_HORIZONTAL_VELOCITY : -TAKING_DAMAGE_INITIAL_HORIZONTAL_VELOCITY;
    velocity.y = TAKING_DAMAGE_INITIAL_VERTICAL_VELOCITY;
    state = HIT_BY_FIREBALL;
}

void Goomba::applyHitByFireballMovement() {
    position.x += velocity.x;

    velocity.y += 0.2f;
    position.y += velocity.y;
}

void Goomba::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    if (state == DEAD) {
        return;
    }

    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    if (state == HIT_BY_FIREBALL) {
        currentAnimator->draw(renderer, &spritePosition, false, true);
    } else {
        currentAnimator->draw(renderer, &spritePosition, false, false);
    }
}
