#ifndef GOOMBA_H
#define GOOMBA_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"
#include "GameObjectsManager.h"
#include "SpriteSheet.h"

class Goomba : public GameObject {

public:

    Goomba(Level* currentLevel, SDL_FPoint* position, GameObjectsManager* objectsManager);
    ~Goomba();

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
    const static float GRAVITY;
    const static Uint64 STOMPED_FRAMES;

    enum GoombaState {
        WALKING,
        STOMPED,
        HIT_BY_FIREBALL,
        DEAD,
    };
    GoombaState state;

    void checkStateTransitions();
    void processCurrentState();

    void applyHorizontalMovement();
    void applyVerticalMovement();
    void resolveCollisions();
    void animateSprite();
    void takeDamage();
    void applyHitByFireballMovement();

    SpriteSheet* spriteSheet;
    Animator* walkingAnimator;
    Animator* stompedAnimator;
    Animator* hitByFireballAnimator;

    Animator* currentAnimator;

    SDL_Point* downCollisionChecks;
    int downCollisionChecksCount;
    SDL_Point* leftCollisionChecks;
    int leftCollisionChecksCount;

    SDL_Rect hitBox;

    Level* currentLevel;

    GameObjectsManager* objectsManager;
    std::vector<GameObject*>* objectsList;
};

#endif
