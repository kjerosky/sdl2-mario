#include "BrickPiece.h"

#include "SpriteSheetRepository.h"

const float BrickPiece::INITIAL_VELOCITY_X = 1.0f;
const float BrickPiece::INITIAL_UPPER_VELOCITY_Y = -6.0f;
const float BrickPiece::INITIAL_LOWER_VELOCITY_Y = -4.5f;
const float BrickPiece::GRAVITY = 0.3f;

BrickPiece::BrickPiece(SDL_Point* initialPosition, BrickPiecePlace direction) {
    position.x = initialPosition->x;
    position.y = initialPosition->y;

    velocity.x = direction == UP_RIGHT || direction == DOWN_RIGHT
        ? INITIAL_VELOCITY_X
        : -INITIAL_VELOCITY_X
    ;
    velocity.y = direction == DOWN_LEFT || direction == DOWN_RIGHT
        ? INITIAL_LOWER_VELOCITY_Y
        : INITIAL_UPPER_VELOCITY_Y
    ;

    SpriteSheet* spriteSheet = SpriteSheetRepository::getInstance()->getSpriteSheetById(BRICK_PIECES_SHEET);

    int frameIndices[] = {0, 1, 2, 3};
    int frameIndicesCount = sizeof(frameIndices) / sizeof(int);
    animator = new Animator(spriteSheet, 8, frameIndices, frameIndicesCount);
}

BrickPiece::~BrickPiece() {
    delete animator;
}

GameObjectType BrickPiece::getType() {
    return DECORATION;
}

SDL_Rect *BrickPiece::getHitBox() {
    return NULL;
}

bool BrickPiece::isCollidable() {
    return false;
}

bool BrickPiece::isStompable() {
    return false;
}

bool BrickPiece::isDrawnOnTop() {
    return true;
}

CollisionResponse BrickPiece::receiveCollision(GameObject *sourceObject) {
    CollisionResponse response = {NO_PROBLEM, this};
    return response;
}

void BrickPiece::update(SDL_Point *cameraPosition) {
    velocity.y += GRAVITY;

    position.x += velocity.x;
    position.y += velocity.y;

    animator->update();
}

void BrickPiece::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Point spritePosition = {
        (int)(position.x - cameraPosition->x),
        (int)(position.y - cameraPosition->y),
    };

    animator->draw(renderer, &spritePosition, false, false);
}
