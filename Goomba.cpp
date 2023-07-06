#include "Goomba.h"

#include <iostream>
#include <SDL_image.h>

#include "Time.h"

Goomba::Goomba(SDL_Renderer* renderer, Level* currentLevel, SDL_FPoint* position, std::vector<GameObject*>* objectsList) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsList = objectsList;

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

    GameObject::CollisionResponse response;
    GameObject::Type sourceType = sourceObject->getType();
    switch (sourceType) {
        case PLAYER:
            if ((int)(sourceObject->getVelocity()->y) > 0) {
                response = REACT_TO_STOMP;

                state = STOMPED;
                stompedTimer = STOMPED_TIME;
            } else {
                response = TAKE_DAMAGE;
            }
            break;
        case ENEMY:
            velocity.x = -velocity.x;
            response = REVERSE_COURSE;
            break;
        default:
            response = NO_PROBLEM;
            break;
    }

    return response;
}

void Goomba::update(SDL_Point *cameraPosition) {
    Animator* previousAnimator = currentAnimator;

    if (state == WALKING) {
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
                    std::cout << "Goomba takes damage" << std::endl;
                    //TODO
                    break;
                default:
                    std::cerr << "[ERROR] Goomba received unknown collision response: " << collisionResponse << std::endl;
                    break;
            }
        }
    } else if (state == STOMPED) {
        currentAnimator = stompedAnimator;
        stompedTimer -= Time::deltaTime * 1000.0f;
        if (stompedTimer <= 0) {
            state = DEAD;
        }
    } else if (state == DEAD) {
        return;
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
