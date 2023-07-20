#ifndef FIREBALL_H
#define FIREBALL_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "GameObjectsManager.h"
#include "Animator.h"

class Fireball : public GameObject {

public:

    Fireball(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, GameObjectsManager* objectsManager, bool facingRight);
    ~Fireball();

    GameObject::Type getType();
    SDL_Rect* getHitBox();
    bool isStompable();
    GameObject::CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    const static float HORIZONTAL_VELOCITY;
    const static float MAX_VERTICAL_VELOCITY;
    const static int MOVING_SPRITE_WIDTH;

    void applyHorizontalMovement();
    void resolveCollisions();
    void beginExploding();

    Level* currentLevel;
    GameObjectsManager* objectsManager;
    std::vector<GameObject*>* objectsList;
    bool facingRight;

    SDL_Texture* spriteSheet;
    Animator* movingAnimator;
    Animator* explodingAnimator;
    Animator* currentAnimator;

    SDL_Rect hitbox;

    SDL_Point* collisionPoints;
    int collisionPointsCount;

    bool exploding;
};

#endif
