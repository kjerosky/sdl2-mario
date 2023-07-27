#include "BlockBumpKillZone.h"

BlockBump::BlockBump() {
    hitbox.x = 0;
    hitbox.y = 0;
    hitbox.w = 16;
    hitbox.h = 16;
}

GameObjectType BlockBump::getType() {
    return BLOCK_BUMP;
}

SDL_Rect* BlockBump::getHitBox() {
    return &hitbox;
}

bool BlockBump::isCollidable() {
    return false;
}

bool BlockBump::isStompable() {
    return false;
}

bool BlockBump::isDrawnOnTop() {
    return false;
}

CollisionResponse BlockBump::receiveCollision(GameObject *sourceObject) {
    // this should never be called
    CollisionResponse response;
    return response;
}

void BlockBump::update(SDL_Point *cameraPosition) {
    // this should never be called
}

void BlockBump::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    // this should never be called
}

void BlockBump::setPosition(SDL_Point* position) {
    this->position.x = position->x;
    this->position.y = position->y;
}
