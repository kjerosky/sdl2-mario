#include "Goomba.h"

#include <iostream>
#include <SDL_image.h>

#include "Time.h"

const float Goomba::HORIZONTAL_VELOCITY = -0.5f;
const float Goomba::GRAVITY = 0.1f;
const Uint64 Goomba::STOMPED_TIME = 1000;

Goomba::Goomba(SDL_Renderer* renderer, Level* currentLevel, SDL_FPoint* position, std::vector<GameObject*>* objectsList) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsList = objectsList;

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

    spriteSheet = IMG_LoadTexture(renderer, "assets/goomba.png");
    if (!spriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    int walkingFrames[] = {0, 1};
    int walkingFramesCount = sizeof(walkingFrames) / sizeof(int);
    walkingAnimator = new Animator(spriteSheet, 16, 16, 0.15f, walkingFrames, walkingFramesCount);

    int stompedFrames[] = {2};
    int stompedFramesCount = sizeof(stompedFrames) / sizeof(int);
    stompedAnimator = new Animator(spriteSheet, 16, 16, 1.0f, stompedFrames, stompedFramesCount);

    currentAnimator = walkingAnimator;

    state = WALKING;
}

Goomba::~Goomba() {
    SDL_DestroyTexture(spriteSheet);

    delete walkingAnimator;
    delete stompedAnimator;

    delete[] downCollisionChecks;
    delete[] leftCollisionChecks;
}

GameObject::Type Goomba::getType() {
    return ENEMY;
}

SDL_Rect* Goomba::getHitBox() {
    return &hitBox;
}

bool Goomba::isStompable() {
    return true;
}

GameObject::CollisionResponse Goomba::receiveCollision(GameObject* sourceObject) {
    if (state == STOMPED || state == DEAD) {
        return NO_PROBLEM;
    }

    GameObject::CollisionResponse response = NO_PROBLEM;
    GameObject::Type sourceType = sourceObject->getType();
    switch (sourceType) {
        case PLAYER: {
            bool isPlayerFalling = sourceObject->getVelocity()->y > 0;
            if (isPlayerFalling) {
                response = REACT_TO_STOMP;

                stompedTimer = STOMPED_TIME;
                state = STOMPED;
            } else {
                response = TAKE_DAMAGE;
            }
        } break;
        case ENEMY: {
            velocity.x = -velocity.x;
            response = REVERSE_COURSE;
        } break;
        default: {
            response = NO_PROBLEM;
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
            // nothing to do - transitions are handled through collisions
            break;
        case STOMPED:
            if (stompedTimer <= 0) {
                state = DEAD;
            }
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
            stompedTimer -= Time::deltaTime * 1000.0f;
            break;
        case DEAD:
            // nothing to do
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
    for (std::vector<GameObject*>::iterator currentObject = objectsList->begin(); currentObject != objectsList->end(); currentObject++) {
        if (*currentObject == this) {
            continue;
        }

        SDL_Rect myWorldHitBox = *getHitBox();
        myWorldHitBox.x += position.x;
        myWorldHitBox.y += position.y;
        SDL_Rect otherWorldHitBox = *((*currentObject)->getHitBox());
        otherWorldHitBox.x += (*currentObject)->getPosition()->x;
        otherWorldHitBox.y += (*currentObject)->getPosition()->y;
        if (!SDL_HasIntersection(&myWorldHitBox, &otherWorldHitBox)) {
            continue;
        }

        GameObject::CollisionResponse collisionResponse = (*currentObject)->receiveCollision(this);
        switch (collisionResponse) {
            case NO_PROBLEM:
                // no need to react
                break;
            case REVERSE_COURSE:
                velocity.x = -velocity.x;
                break;
            case GET_STOMPED:
                state = STOMPED;
                stompedTimer = STOMPED_TIME;
                break;
            case TAKE_DAMAGE:
                //TODO
                std::cout << "Goomba takes damage" << std::endl;
                break;
            default:
                std::cerr << "[ERROR] Goomba received unknown collision response: " << collisionResponse << std::endl;
                break;
        }
    }
}

void Goomba::animateSprite() {
    Animator* previousAnimator = currentAnimator;

    if (state == STOMPED) {
        currentAnimator = stompedAnimator;
    }

    if (previousAnimator != currentAnimator) {
        currentAnimator->reset();
    } else {
        currentAnimator->update();
    }
}

void Goomba::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    if (state == DEAD) {
        return;
    }

    SDL_Point spritePosition = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y
    };

    currentAnimator->draw(renderer, &spritePosition, false);
}
