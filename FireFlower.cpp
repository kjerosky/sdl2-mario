#include "FireFlower.h"

#include "SpriteSheetRepository.h"

const int FireFlower::SPAWN_OFFSET_Y = -4;
const int FireFlower::SPRITE_HEIGHT = 16;
const float FireFlower::LEAVING_BLOCK_VELOCITY_Y = -0.25f;

FireFlower::FireFlower(Level* level, SDL_FPoint* blockPosition, GameObjectsManager* objectsManager) {
    this->level = level;
    this->objectsManager = objectsManager;

    position.x = blockPosition->x;
    position.y = blockPosition->y + SPAWN_OFFSET_Y;

    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 16;
    hitbox.h = 16;

    spriteSheet = SpriteSheetRepository::getInstance()->getSpriteSheetById(POWERUPS_SHEET);
    int frameIndices[] = {1, 2, 3, 4};
    int frameIndicesCount = sizeof(frameIndices) / sizeof(int);
    animator = new Animator(spriteSheet, 2, frameIndices, frameIndicesCount);

    state = LEAVING_BLOCK;
    leavingBlockTargetY = (int)(blockPosition->y) - SPRITE_HEIGHT;
}

FireFlower::~FireFlower() {
    delete animator;
}

GameObjectType FireFlower::getType() {
    return POWERUP;
}

SDL_Rect* FireFlower::getHitBox() {
    return &hitbox;
}

bool FireFlower::isCollidable() {
    return true;
}

bool FireFlower::isStompable() {
    return false;
}

bool FireFlower::isDrawnOnTop() {
    return false;
}

CollisionResponse FireFlower::receiveCollision(GameObject* sourceObject) {
    CollisionResponse response = {NO_PROBLEM, this};
    if (sourceObject->getType() == PLAYER) {
        getConsumedByPlayer();
        response.type = POWER_UP;
    }

    return response;
}

void FireFlower::update(SDL_Point* cameraPosition) {
    checkStateTransitions();
    processCurrentState();

    animator->update();
}

void FireFlower::checkStateTransitions() {
    if (state == LEAVING_BLOCK && position.y <= leavingBlockTargetY) {
        position.y = leavingBlockTargetY;
        state = STANDING_STILL;
    }
}

void FireFlower::processCurrentState() {
    if (state == LEAVING_BLOCK) {
        position.y += LEAVING_BLOCK_VELOCITY_Y;
    }
}

void FireFlower::getConsumedByPlayer() {
    if (state == CONSUMED) {
        return;
    }

    state = CONSUMED;
    objectsManager->destroy(this);
}

void FireFlower::draw(SDL_Renderer* renderer, SDL_Point* cameraPosition) {
    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    animator->draw(renderer, &spritePosition, false, false);
}
