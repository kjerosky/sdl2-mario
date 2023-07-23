#ifndef BRICK_PIECE_H
#define BRICK_PIECE_H

#include "GameObject.h"
#include "Animator.h"

enum BrickPiecePlace {
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
};

class BrickPiece : public GameObject {

public:

    BrickPiece(SDL_Point* initialPosition, BrickPiecePlace direction);
    ~BrickPiece();

    GameObjectType getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    const static float INITIAL_VELOCITY_X;
    const static float INITIAL_UPPER_VELOCITY_Y;
    const static float INITIAL_LOWER_VELOCITY_Y;
    const static float GRAVITY;

    Animator* animator;
};

#endif
