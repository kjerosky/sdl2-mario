#include "Player.h"

#include <iostream>

#include "GameConfig.h"
#include "Fireball.h"
#include "SpriteSheetRepository.h"

const float Player::JUMP_VELOCITY = -3.7f;
const float Player::NORMAL_BONK_VELOCITY = 3.0f;
const float Player::SLOW_BONK_VELOCITY = 0.5f;
const float Player::JUMP_GRAVITY = 0.1f;
const float Player::FALL_GRAVITY = 0.3f;
const float Player::STOMP_REACTION_VELOCITY = -3.0f;
const int Player::SPRITE_WIDTH = 16;
const int Player::THROW_FIREBALL_FRAME_COUNT = 7;
const int Player::INVINCIBILITY_FRAMES_COUNT = 180;
const int Player::BONK_POINT_X = 8;

Player::Player(Level *currentLevel, SDL_FPoint *position, GameObjectsManager* objectsManager, LevelAnimator* levelAnimator) {
    this->position = *position;
    this->currentLevel = currentLevel;
    this->objectsManager = objectsManager;
    objectsList = objectsManager->getObjectList();
    this->levelAnimator = levelAnimator;

    enabled = true;

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
    smallSizeRightCollisionChecks = new SDL_Point[smallSizeRightCollisionChecksCount];
    smallSizeRightCollisionChecks[0].x = 13;
    smallSizeRightCollisionChecks[0].y = 0;
    smallSizeRightCollisionChecks[1].x = 13;
    smallSizeRightCollisionChecks[1].y = 15;

    poweredUpDownCollisionChecksCount = 2;
    poweredUpDownCollisionChecks = new SDL_Point[poweredUpDownCollisionChecksCount];
    poweredUpDownCollisionChecks[0].x = 2;
    poweredUpDownCollisionChecks[0].y = 31;
    poweredUpDownCollisionChecks[1].x = 13;
    poweredUpDownCollisionChecks[1].y = 31;

    poweredUpRightCollisionChecksCount = 4;
    poweredUpRightCollisionChecks = new SDL_Point[poweredUpRightCollisionChecksCount];
    poweredUpRightCollisionChecks[0].x = 13;
    poweredUpRightCollisionChecks[0].y = 0;
    poweredUpRightCollisionChecks[1].x = 13;
    poweredUpRightCollisionChecks[1].y = 10;
    poweredUpRightCollisionChecks[2].x = 13;
    poweredUpRightCollisionChecks[2].y = 20;
    poweredUpRightCollisionChecks[3].x = 13;
    poweredUpRightCollisionChecks[3].y = 31;

    currentDownCollisionChecksCount = smallSizeDownCollisionChecksCount;
    currentDownCollisionChecks = &smallSizeDownCollisionChecks;
    currentRightCollisionChecksCount = smallSizeRightCollisionChecksCount;
    currentRightCollisionChecks = &smallSizeRightCollisionChecks;

    smallMarioHitBox.x = 2;
    smallMarioHitBox.y = 0;
    smallMarioHitBox.w = 12;
    smallMarioHitBox.h = 16;

    poweredUpMarioHitBox.x = 2;
    poweredUpMarioHitBox.y = 0;
    poweredUpMarioHitBox.w = 12;
    poweredUpMarioHitBox.h = 32;

    currentHitBox = &smallMarioHitBox;

    SpriteSheetRepository* spriteSheetRepository = SpriteSheetRepository::getInstance();
    smallMarioSpriteSheet = spriteSheetRepository->getSpriteSheetById(SMALL_MARIO_SHEET);
    superMarioSpriteSheet = spriteSheetRepository->getSpriteSheetById(SUPER_MARIO_SHEET);
    fireMarioSpriteSheet = spriteSheetRepository->getSpriteSheetById(FIRE_MARIO_SHEET);

    int smallMarioStandingFrames[] = {0};
    int smallMarioStandingFramesCount = sizeof(smallMarioStandingFrames) / sizeof(int);
    smallMarioStandingAnimator = new Animator(smallMarioSpriteSheet, 1, smallMarioStandingFrames, smallMarioStandingFramesCount);

    int smallMarioWalkingFrames[] = {1, 2, 3, 2};
    int smallMarioWalkingFramesCount = sizeof(smallMarioWalkingFrames) / sizeof(int);
    smallMarioWalkingAnimator = new Animator(smallMarioSpriteSheet, 6, smallMarioWalkingFrames, smallMarioWalkingFramesCount);

    int smallMarioJumpingFrames[] = {5};
    int smallMarioJumpingFramesCount = sizeof(smallMarioJumpingFrames) / sizeof(int);
    smallMarioJumpingAnimator = new Animator(smallMarioSpriteSheet, 1, smallMarioJumpingFrames, smallMarioJumpingFramesCount);

    int superMarioStandingFrames[] = {0};
    int superMarioStandingFramesCount = sizeof(superMarioStandingFrames) / sizeof(int);
    superMarioStandingAnimator = new Animator(superMarioSpriteSheet, 1, superMarioStandingFrames, superMarioStandingFramesCount);

    int superMarioWalkingFrames[] = {3, 1, 2};
    int superMarioWalkingFramesCount = sizeof(superMarioWalkingFrames) / sizeof(int);
    superMarioWalkingAnimator = new Animator(superMarioSpriteSheet, 6, superMarioWalkingFrames, superMarioWalkingFramesCount);

    int superMarioJumpingFrames[] = {5};
    int superMarioJumpingFramesCount = sizeof(superMarioJumpingFrames) / sizeof(int);
    superMarioJumpingAnimator = new Animator(superMarioSpriteSheet, 1, superMarioJumpingFrames, superMarioJumpingFramesCount);

    int fireMarioStandingFrames[] = {0};
    int fireMarioStandingFramesCount = sizeof(fireMarioStandingFrames) / sizeof(int);
    fireMarioStandingAnimator = new Animator(fireMarioSpriteSheet, 1, fireMarioStandingFrames, fireMarioStandingFramesCount);

    int fireMarioWalkingFrames[] = {3, 1, 2};
    int fireMarioWalkingFramesCount = sizeof(fireMarioWalkingFrames) / sizeof(int);
    fireMarioWalkingAnimator = new Animator(fireMarioSpriteSheet, 6, fireMarioWalkingFrames, fireMarioWalkingFramesCount);

    int fireMarioJumpingFrames[] = {5};
    int fireMarioJumpingFramesCount = sizeof(fireMarioJumpingFrames) / sizeof(int);
    fireMarioJumpingAnimator = new Animator(fireMarioSpriteSheet, 1, fireMarioJumpingFrames, fireMarioJumpingFramesCount);

    int fireMarioThrowingFireballFrames[] = {15};
    int fireMarioThrowingFireballCount = sizeof(fireMarioThrowingFireballFrames) / sizeof(int);
    fireMarioThrowingFireballAnimator = new Animator(fireMarioSpriteSheet, 1, fireMarioThrowingFireballFrames, fireMarioThrowingFireballCount);

    powerState = SMALL_MARIO;
    currentAnimator = smallMarioStandingAnimator;
    currentSpriteHeight = 16;

    state = ON_GROUND;

    throwFireballFramesLeft = 0;

    input = Input::getInstance();

    invincibilityFramesLeft = 0;
}

Player::~Player() {
    delete[] smallSizeDownCollisionChecks;
    delete[] smallSizeRightCollisionChecks;

    delete[] poweredUpDownCollisionChecks;
    delete[] poweredUpRightCollisionChecks;

    delete smallMarioStandingAnimator;
    delete smallMarioWalkingAnimator;
    delete smallMarioJumpingAnimator;

    delete superMarioStandingAnimator;
    delete superMarioWalkingAnimator;
    delete superMarioJumpingAnimator;

    delete fireMarioStandingAnimator;
    delete fireMarioWalkingAnimator;
    delete fireMarioJumpingAnimator;
    delete fireMarioThrowingFireballAnimator;
}

GameObjectType Player::getType() {
    return PLAYER;
}

SDL_Rect* Player::getHitBox() {
    return currentHitBox;
}

bool Player::isCollidable() {
    return true;
}

bool Player::isStompable() {
    return true;
}

bool Player::isDrawnOnTop() {
    return true;
}

CollisionResponse Player::receiveCollision(GameObject* sourceObject) {
    CollisionResponse response;
    GameObjectType sourceType = sourceObject->getType();
    switch (sourceType) {
        case ENEMY:
            if (sourceObject->isStompable() && velocity.y > 0) {
                velocity.y = STOMP_REACTION_VELOCITY; // fake bounce velocity
                response = GET_STOMPED;
            } else {
                response = NO_PROBLEM;
                takeDamage();
            }
            break;

        case POWERUP:
            powerUp();
            response = GET_CONSUMED;
            break;

        default:
            response = NO_PROBLEM;
            break;
    }

    return response;
}

void Player::update(SDL_Point *cameraPosition) {
    updateGroundStatus();
    checkStateTransitions();
    processCurrentState();
    resolveCollisions();
    animateSprite();

    centerCameraOnPlayer(cameraPosition);
}

void Player::checkStateTransitions() {
    switch (state) {
        case ON_GROUND:
            if (input->jumpWasPressed()) {
                velocity.y = JUMP_VELOCITY;
                isGrounded = false;
                state = JUMPING;
            } else if (!isGrounded) {
                state = FALLING;
            }
            break;
        case JUMPING:
            if (input->jumpWasReleased() || velocity.y > 0) {
                state = FALLING;
            } else if (isGrounded) {
                state = ON_GROUND;
            }
            break;
        case FALLING:
            if (isGrounded) {
                state = ON_GROUND;
            }
            break;
    }
}

void Player::processCurrentState() {
    switch (state) {
        case ON_GROUND:
            applyHorizontalMovement();
            applyVerticalMovement(FALL_GRAVITY);
            attemptFireballThrow();
            break;
        case JUMPING:
            applyHorizontalMovement();
            applyVerticalMovement(JUMP_GRAVITY);
            attemptFireballThrow();
            break;
        case FALLING:
            applyHorizontalMovement();
            applyVerticalMovement(FALL_GRAVITY);
            attemptFireballThrow();
            break;
    }

    if (invincibilityFramesLeft > 0) {
        invincibilityFramesLeft--;
    }
}

void Player::updateGroundStatus() {
    isGrounded = false;
    for (int i = 0; i < currentDownCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = (*currentDownCollisionChecks)[i];
        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y + 1,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint) && velocity.y >= 0) {
            isGrounded = true;
            velocity.y = 0;
            break;
        }
    }
}

void Player::applyHorizontalMovement() {
    int horizontalInput = (input->rightIsHeld() ? 1 : 0) - (input->leftIsHeld() ? 1 : 0);

    //TODO THIS IS TESTING ONLY
    if (horizontalInput != 0) {
        velocity.x = 1 * horizontalInput; //fake horizontal velocity
        facingRight = velocity.x > 0;
    } else {
        velocity.x = 0;
    }
    position.x += velocity.x;
    for (int i = 0; i < currentRightCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = (*currentRightCollisionChecks)[i];
        if (!facingRight) {
            collisionCheckPoint.x = (SPRITE_WIDTH - 1) - collisionCheckPoint.x;
        }

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.x = 0;
            position.x = (int)position.x;

            do {
                position.x += facingRight ? -1 : 1;
                testPoint.x += facingRight ? -1 : 1;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }
}

void Player::applyVerticalMovement(float gravity) {
    if (isGrounded) {
        return;
    }

    velocity.y += gravity;
    position.y += velocity.y;

    if (velocity.y < 0) {
        checkUpwardMovement();
    } else {
        checkDownwardMovement();
    }
}

void Player::checkUpwardMovement() {
    SDL_Point testPoint = {
        (int)position.x + (facingRight ? BONK_POINT_X : (SPRITE_WIDTH - 1) - BONK_POINT_X),
        (int)position.y,
    };
    if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
        velocity.y =
            levelAnimator->animatePlayerBonk(&testPoint, powerState != SMALL_MARIO)
            ? SLOW_BONK_VELOCITY
            : NORMAL_BONK_VELOCITY;
        position.y = (int)position.y;

        do {
            position.y++;
            testPoint.y++;
        } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
        return;
    }

    for (int i = 0; i < currentDownCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = (*currentDownCollisionChecks)[i];
        collisionCheckPoint.y = (currentSpriteHeight - 1) - collisionCheckPoint.y;

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (currentLevel->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.y =
                levelAnimator->animatePlayerBonk(&testPoint, powerState != SMALL_MARIO)
                ? SLOW_BONK_VELOCITY
                : NORMAL_BONK_VELOCITY;
            position.y = (int)position.y;

            do {
                position.y++;
                testPoint.y = (int)position.y + collisionCheckPoint.y;
            } while (currentLevel->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }
}

void Player::checkDownwardMovement() {
    for (int i = 0; i < currentDownCollisionChecksCount; i++) {
        SDL_Point collisionCheckPoint = (*currentDownCollisionChecks)[i];
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

void Player::resolveCollisions() {
    for (std::vector<GameObject*>::iterator currentObject = objectsList->begin(); currentObject != objectsList->end(); currentObject++) {
        if (*currentObject == this || !(*currentObject)->isEnabled() || !(*currentObject)->isCollidable()) {
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

        CollisionResponse collisionResponse = (*currentObject)->receiveCollision(this);
        switch (collisionResponse) {
            case NO_PROBLEM:
                // no need to react
                break;

            case REACT_TO_STOMP:
                velocity.y = STOMP_REACTION_VELOCITY; // fake bounce velocity
                break;

            case TAKE_DAMAGE:
                takeDamage();
                break;

            case POWER_UP:
                powerUp();
                break;

            default:
                std::cerr << "[ERROR] Player received unknown collision response: " << collisionResponse << std::endl;
                break;
        }
    }

    if (powerState != FIRE_MARIO && input->DEBUG_plus_buttonWasPressed()) {
        powerUp();
    } else if (powerState != SMALL_MARIO && input->DEBUG_minus_buttonWasPressed()) {
        powerDown();
    }
}

void Player::animateSprite() {
    Animator *previousAnimator = currentAnimator;

    switch (powerState) {
        case SMALL_MARIO: {
            if (state == ON_GROUND) {
                if (velocity.x != 0) {
                    currentAnimator = smallMarioWalkingAnimator;
                } else {
                    currentAnimator = smallMarioStandingAnimator;
                }
            } else {
                currentAnimator = smallMarioJumpingAnimator;
            }
        } break;

        case SUPER_MARIO: {
            if (state == ON_GROUND) {
                if (velocity.x != 0) {
                    currentAnimator = superMarioWalkingAnimator;
                } else {
                    currentAnimator = superMarioStandingAnimator;
                }
            } else {
                currentAnimator = superMarioJumpingAnimator;
            }
        } break;

        case FIRE_MARIO: {
            if (throwFireballFramesLeft > 0) {
                currentAnimator = fireMarioThrowingFireballAnimator;
                throwFireballFramesLeft--;
            } else if (state == ON_GROUND) {
                if (velocity.x != 0) {
                    currentAnimator = fireMarioWalkingAnimator;
                } else {
                    currentAnimator = fireMarioStandingAnimator;
                }
            } else {
                currentAnimator = fireMarioJumpingAnimator;
            }
        } break;
    }

    if (previousAnimator != currentAnimator) {
        currentAnimator->reset();
    } else {
        currentAnimator->update();
    }
}

void Player::centerCameraOnPlayer(SDL_Point* cameraPosition) {
    int renderWidth = GameConfig::getInstance()->getRenderWidth();
    int newCameraPositionX = position.x + 16 / 2 - renderWidth / 2;

    // camera can only move right
    if (newCameraPositionX > cameraPosition->x) {
        cameraPosition->x = newCameraPositionX;
    }

    currentLevel->constrainCameraToLevel(cameraPosition);

    // constrain player to camera's width
    if (position.x < cameraPosition->x) {
        position.x = cameraPosition->x;
    } else if (position.x + SPRITE_WIDTH > cameraPosition->x + renderWidth) {
        position.x = cameraPosition->x + renderWidth - SPRITE_WIDTH;
    }
}

void Player::attemptFireballThrow() {
    if (powerState != FIRE_MARIO || !input->fireWasPressed()) {
        return;
    }

    SDL_FPoint fireballSpawnPosition = {
        facingRight ? position.x + 8 : position.x - 8,
        position.y + 8
    };

    Fireball* fireball = new Fireball(currentLevel, &fireballSpawnPosition, objectsManager, facingRight);
    fireball->enable();
    objectsManager->add(fireball);

    throwFireballFramesLeft = THROW_FIREBALL_FRAME_COUNT;
}

void Player::takeDamage() {
    if (invincibilityFramesLeft > 0) {
        return;
    }

    if (powerState == SMALL_MARIO) {
        //TODO
        std::cerr << "mario died" << std::endl;
    } else {
        powerDown();
        invincibilityFramesLeft = INVINCIBILITY_FRAMES_COUNT;
    }
}

void Player::powerUp()
{
    if (powerState == SMALL_MARIO) {
        position.y -= 16;
        currentSpriteHeight = 32;
        currentDownCollisionChecksCount = poweredUpDownCollisionChecksCount;
        currentDownCollisionChecks = &poweredUpDownCollisionChecks;
        currentRightCollisionChecksCount = poweredUpRightCollisionChecksCount;
        currentRightCollisionChecks = &poweredUpRightCollisionChecks;
        currentHitBox = &poweredUpMarioHitBox;
        powerState = SUPER_MARIO;
    } else {
        powerState = FIRE_MARIO;
    }
}

void Player::powerDown() {
    if (powerState == SUPER_MARIO) {
        position.y += 16;
        currentSpriteHeight = 16;
        currentDownCollisionChecksCount = smallSizeDownCollisionChecksCount;
        currentDownCollisionChecks = &smallSizeDownCollisionChecks;
        currentRightCollisionChecksCount = smallSizeRightCollisionChecksCount;
        currentRightCollisionChecks = &smallSizeRightCollisionChecks;
        currentHitBox = &smallMarioHitBox;
        powerState = SMALL_MARIO;
    } else {
        powerState = SUPER_MARIO;
    }
}

void Player::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    if (invincibilityFramesLeft <= 0 || invincibilityFramesLeft % 2 == 0) {
        currentAnimator->draw(renderer, &spritePosition, !facingRight, false);
    }
}
