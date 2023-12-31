#ifndef FIREBALL_H
#define FIREBALL_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "GameObjectsManager.h"
#include "Animator.h"
#include "SpriteSheet.h"
#include "CollisionSystem.h"

class Fireball : public GameObject {

public:

    Fireball(Level *currentLevel, SDL_FPoint *position, GameObjectsManager* objectsManager, bool facingRight);
    ~Fireball();

    GameObjectType getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    const static float HORIZONTAL_VELOCITY;
    const static float MAX_VERTICAL_VELOCITY;
    const static float GRAVITY;
    const static int MOVING_SPRITE_WIDTH;

    void applyHorizontalMovement();
    void applyVerticalMovement();
    void resolveCollisions();
    void beginExploding();

    Level* currentLevel;
    GameObjectsManager* objectsManager;
    bool facingRight;

    SpriteSheet* spriteSheet;
    Animator* movingAnimator;
    Animator* explodingAnimator;
    Animator* currentAnimator;

    SDL_Rect hitbox;
    SDL_Point collisionPoint;

    bool exploding;

    CollisionSystem* collisionSystem;
};

#endif
