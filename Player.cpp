#include "Player.h"

#include <SDL_image.h>
#include <iostream>

#include "GameConfig.h"
#include "Input.h"

Player::Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, std::vector<GameObject*>* objectsList) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsList = objectsList;

    velocity.x = 0;
    velocity.y = 0;

    facingRight = true;

    isGrounded = true;

    smallSizeDownCollisionChecksCount = 2;
    smallSizeDownCollisionChecks = new SDL_Point[smallSizeDownCollisionChecksCount];
    smallSizeDownCollisionChecks[0].x = 2;
    smallSizeDownCollisionChecks[0].y = 15;
    smallSizeDownCollisionChecks[1].x = 13;
    smallSizeDownCollisionChecks[1].y = 15;

    smallSizeRightCollisionChecksCount = 2;
    smallSizeRightCollisionChecks = new SDL_Point[smallSizeDownCollisionChecksCount];
    smallSizeRightCollisionChecks[0].x = 13;
    smallSizeRightCollisionChecks[0].y = 0;
    smallSizeRightCollisionChecks[1].x = 13;
    smallSizeRightCollisionChecks[1].y = 15;

    smallMarioHitBox.x = 2;
    smallMarioHitBox.y = 0;
    smallMarioHitBox.w = 12;
    smallMarioHitBox.h = 16;

    currentHitBox = &smallMarioHitBox;

    smallMarioSpriteSheet = IMG_LoadTexture(renderer, "assets/small-mario.png");
    if (!smallMarioSpriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    int smallMarioStandingFrames[] = {0};
    int smallMarioStandingFramesCount = sizeof(smallMarioStandingFrames) / sizeof(int);
    smallMarioStandingAnimator = new Animator(smallMarioSpriteSheet, 16, 16, 1.0f, smallMarioStandingFrames, smallMarioStandingFramesCount);

    int smallMarioWalkingFrames[] = {1, 2, 3, 2};
    int smallMarioWalkingFramesCount = sizeof(smallMarioWalkingFrames) / sizeof(int);
    smallMarioWalkingAnimator = new Animator(smallMarioSpriteSheet, 16, 16, 0.1f, smallMarioWalkingFrames, smallMarioWalkingFramesCount);

    int smallMarioJumpingFrames[] = {5};
    int smallMarioJumpingFramesCount = sizeof(smallMarioJumpingFrames) / sizeof(int);
    smallMarioJumpingAnimator = new Animator(smallMarioSpriteSheet, 16, 16, 0.1f, smallMarioJumpingFrames, smallMarioJumpingFramesCount);

    currentAnimator = smallMarioStandingAnimator;
}

Player::~Player() {
    SDL_DestroyTexture(smallMarioSpriteSheet);

    delete[] smallSizeDownCollisionChecks;
    delete[] smallSizeRightCollisionChecks;

    delete smallMarioStandingAnimator;
    delete smallMarioWalkingAnimator;
    delete smallMarioJumpingAnimator;
}

GameObject::Type Player::getType() {
    return PLAYER;
}

SDL_Rect* Player::getHitBox() {
    return currentHitBox;
}

bool Player::isStompable() {
    return true;
}

GameObject::CollisionResponse Player::receiveCollision(GameObject* sourceObject) {
    GameObject::CollisionResponse response;
    GameObject::Type sourceType = sourceObject->getType();
    switch (sourceType) {
        case ENEMY:
            if (sourceObject->isStompable() && velocity.y > 0) {
                response = GET_STOMPED;
            } else {
                response = NO_PROBLEM;
                std::cout << "Player taking damage from enemy!" << std::endl;
            }
            break;
        default:
            response = NO_PROBLEM;
            break;
    }

    return response;
}

void Player::update(SDL_Point *cameraPosition) {
    Animator *previousAnimator = currentAnimator;

    int renderWidth = GameConfig::getInstance()->getRenderWidth();

    Input *input = Input::getInstance();
    int horizontalInput = (input->rightIsHeld() ? 1 : 0) - (input->leftIsHeld() ? 1 : 0);

    //TODO THIS IS TESTING ONLY
    if (horizontalInput != 0) {
        velocity.x = 1 * horizontalInput; //fake horizontal velocity
        facingRight = velocity.x > 0;
    } else {
        velocity.x = 0;
    }
    position.x += velocity.x;
    for (int i = 0; i < smallSizeRightCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = smallSizeRightCollisionChecks[i];
        if (!facingRight) {
            collisionCheckPoint.x = 15 - collisionCheckPoint.x;
        }

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.x = 0;
            position.x = (int)position.x;

            testPoint.y = (int)position.y + collisionCheckPoint.y;
            do {
                position.x += facingRight ? -1 : 1;
                testPoint.x = (int)position.x + collisionCheckPoint.x;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }

    //TODO THIS IS TESTING ONLY
    isGrounded = false;
    for (int i = 0; i < smallSizeDownCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = smallSizeDownCollisionChecks[i];
        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y + 1,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint) && velocity.y >= 0) {
            velocity.y = 0;
            isGrounded = true;
            break;
        }
    }
    if (isGrounded) {
        if (input->jumpWasPressed()) {
            velocity.y = -3.7; //fake jump velocity
        }
    } else {
        velocity.y += 0.1; //fake gravity effect
    }
    position.y += velocity.y;
    bool isMovingUpwards = velocity.y < 0;
    for (int i = 0; i < smallSizeDownCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = smallSizeDownCollisionChecks[i];
        if (isMovingUpwards) {
            collisionCheckPoint.y = 15 - collisionCheckPoint.y;
        }

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.y = isMovingUpwards ? 3 : 0;
            position.y = (int)position.y;

            testPoint.x = (int)position.x + collisionCheckPoint.x;
            do {
                position.y += isMovingUpwards ? 1 : -1;
                testPoint.y = (int)position.y + collisionCheckPoint.y;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }

    cameraPosition->x = position.x + 16 / 2 - renderWidth / 2;

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
            case REACT_TO_STOMP:
                velocity.y = -2.0f; // fake bounce velocity
                break;
            case TAKE_DAMAGE:
                std::cout << "player takes damage" << std::endl;
                //TODO
                break;
            default:
                std::cerr << "[ERROR] Player received unknown collision response: " << collisionResponse << std::endl;
                break;
        }
    }

    if (isGrounded) {
        if (velocity.x != 0) {
            currentAnimator = smallMarioWalkingAnimator;
        } else {
            currentAnimator = smallMarioStandingAnimator;
        }
    } else {
        currentAnimator = smallMarioJumpingAnimator;
    }

    if (previousAnimator != currentAnimator) {
        currentAnimator->reset();
    } else {
        currentAnimator->update();
    }
}

void Player::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y
    };

    currentAnimator->draw(renderer, &spritePosition, !facingRight);
}
