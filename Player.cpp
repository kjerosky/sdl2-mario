#include "Player.h"

#include <SDL_image.h>
#include <iostream>

#include "GameConfig.h"
#include "Input.h"

Player::Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position) {
    this->currentLevel = currentLevel;
    this->position = *position;

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

    spriteSheet = IMG_LoadTexture(renderer, "assets/mario.png");
    if (!spriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }
}

Player::~Player() {
    SDL_DestroyTexture(spriteSheet);
}

void Player::update(SDL_Point *cameraPosition) {
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
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            isGrounded = true;
            break;
        }
    }
    if (isGrounded && input->jumpWasPressed()) {
        velocity.y = -3.7; //fake jump velocity
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
}

void Player::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Rect destinationRectangle = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y,
        16,
        16
    };
    SDL_RenderCopyEx(renderer, spriteSheet, NULL, &destinationRectangle, 0, NULL, facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}
