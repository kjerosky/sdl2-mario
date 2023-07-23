#include "MagicMushroom.h"

#include "SpriteSheetRepository.h"

const float MagicMushroom::INITIAL_VELOCITY_X = 1.0f;
const float MagicMushroom::GRAVITY = 0.2f;
const int MagicMushroom::SPAWN_OFFSET_Y = -4;
const int MagicMushroom::SPRITE_WIDTH = 16;
const int MagicMushroom::SPRITE_HEIGHT = 16;
const float MagicMushroom::LEAVING_BLOCK_VELOCITY_Y = -0.25f;

MagicMushroom::MagicMushroom(Level* level, SDL_FPoint* blockPosition, GameObjectsManager* objectsManager) {
    this->level = level;
    this->objectsManager = objectsManager;
    objectsList = objectsManager->getObjectList();

    position.x = blockPosition->x;
    position.y = blockPosition->y + SPAWN_OFFSET_Y;

    velocity.x = INITIAL_VELOCITY_X;
    velocity.y = 0;

    rightCollisionPointsCount = 2;
    rightCollisionPoints = new SDL_Point[rightCollisionPointsCount];
    rightCollisionPoints[0].x = 15;
    rightCollisionPoints[0].y = 0;
    rightCollisionPoints[1].x = 15;
    rightCollisionPoints[1].y = 15;

    bottomCollisionPointsCount = 2;
    bottomCollisionPoints = new SDL_Point[bottomCollisionPointsCount];
    bottomCollisionPoints[0].x = 0;
    bottomCollisionPoints[0].y = 15;
    bottomCollisionPoints[1].x = 15;
    bottomCollisionPoints[1].y = 15;

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 16;
    hitbox.h = 16;

    spriteSheet = SpriteSheetRepository::getInstance()->getSpriteSheetById(POWERUPS_SHEET);
    int frameIndices[] = {0};
    int frameIndicesCount = sizeof(frameIndices) / sizeof(int);
    animator = new Animator(spriteSheet, 1, frameIndices, frameIndicesCount);

    state = LEAVING_BLOCK;
    leavingBlockTargetY = (int)(blockPosition->y) - SPRITE_HEIGHT;
}

MagicMushroom::~MagicMushroom() {
    delete[] rightCollisionPoints;
    delete[] bottomCollisionPoints;

    delete animator;
}

GameObjectType MagicMushroom::getType() {
    return POWERUP;
}

SDL_Rect *MagicMushroom::getHitBox() {
    return &hitbox;
}

bool MagicMushroom::isCollidable() {
    return true;
}

bool MagicMushroom::isStompable() {
    return false;
}

bool MagicMushroom::isDrawnOnTop() {
    return false;
}

CollisionResponse MagicMushroom::receiveCollision(GameObject* sourceObject) {
    CollisionResponse response = NO_PROBLEM;
    if (sourceObject->getType() == PLAYER) {
        getConsumedByPlayer();
        response = POWER_UP;
    }

    return response;
}

void MagicMushroom::update(SDL_Point* cameraPosition) {
    checkStateTransitions();
    processCurrentState();
}

void MagicMushroom::checkStateTransitions() {
    if (state == LEAVING_BLOCK && position.y <= leavingBlockTargetY) {
        position.y = leavingBlockTargetY;
        state = MOVING;
    }
}

void MagicMushroom::processCurrentState() {
    if (state == LEAVING_BLOCK) {
        position.y += LEAVING_BLOCK_VELOCITY_Y;
    } else if (state == MOVING) {
        applyHorizontalMovement();
        applyVerticalMovement();
        resolveCollisions();
    }
}

void MagicMushroom::applyHorizontalMovement() {
    position.x += velocity.x;
    for (int i = 0; i < rightCollisionPointsCount; i++) {
        SDL_Point collisionCheckPoint = rightCollisionPoints[i];
        if (velocity.x < 0) {
            collisionCheckPoint.x = (SPRITE_WIDTH - 1) - collisionCheckPoint.x;
        }

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (level->isWorldPositionInForegroundTile(&testPoint)) {
            position.x = (int)position.x;

            testPoint.y = (int)position.y + collisionCheckPoint.y;
            do {
                position.x += velocity.x < 0 ? 1 : -1;
                testPoint.x = (int)position.x + collisionCheckPoint.x;
            } while (level->isWorldPositionInForegroundTile(&testPoint));

            velocity.x = -velocity.x;
            break;
        }
    }
}

void MagicMushroom::applyVerticalMovement() {
    velocity.y += GRAVITY;
    position.y += velocity.y;
    for (int i = 0; i < bottomCollisionPointsCount; i++) {
        SDL_Point collisionCheckPoint = bottomCollisionPoints[i];

        SDL_Point testPoint = {
            (int)position.x + collisionCheckPoint.x,
            (int)position.y + collisionCheckPoint.y,
        };
        if (level->isWorldPositionInForegroundTile(&testPoint)) {
            velocity.y = 0;
            position.y = (int)position.y;

            testPoint.x = (int)position.x + collisionCheckPoint.x;
            do {
                position.y--;
                testPoint.y = (int)position.y + collisionCheckPoint.y;
            } while (level->isWorldPositionInForegroundTile(&testPoint));
            break;
        }
    }
}

void MagicMushroom::resolveCollisions() {
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
        if (collisionResponse == GET_CONSUMED) {
            getConsumedByPlayer();
            break;
        }
    }
}

void MagicMushroom::getConsumedByPlayer() {
    if (state == CONSUMED) {
        return;
    }

    state = CONSUMED;
    objectsManager->destroy(this);
}

void MagicMushroom::draw(SDL_Renderer* renderer, SDL_Point* cameraPosition) {
    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    animator->draw(renderer, &spritePosition, false, false);
}
