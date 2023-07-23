#ifndef MAGIC_MUSHROOM_H
#define MAGIC_MUSHROOM_H

#include <SDL.h>

#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "SpriteSheet.h"
#include "Animator.h"
#include "GameObjectsManager.h"

class MagicMushroom : public GameObject {

public:

    MagicMushroom(Level* level, SDL_FPoint* blockPosition, GameObjectsManager* objectsManager);
    ~MagicMushroom();

    GameObjectType getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point* cameraPosition);
    void draw(SDL_Renderer* renderer, SDL_Point* cameraPosition);

private:

    const static float INITIAL_VELOCITY_X;
    const static float GRAVITY;
    const static int SPAWN_OFFSET_Y;
    const static int SPRITE_WIDTH;
    const static int SPRITE_HEIGHT;
    const static float LEAVING_BLOCK_VELOCITY_Y;

    enum MagicMushroomState {
        LEAVING_BLOCK,
        MOVING,
        CONSUMED,
    };
    MagicMushroomState state;

    void checkStateTransitions();
    void processCurrentState();
    void applyHorizontalMovement();
    void applyVerticalMovement();
    void resolveCollisions();
    void getConsumedByPlayer();

    Level* level;

    int rightCollisionPointsCount;
    SDL_Point* rightCollisionPoints;
    int bottomCollisionPointsCount;
    SDL_Point* bottomCollisionPoints;

    SDL_Rect hitbox;

    SpriteSheet* spriteSheet;
    Animator* animator;

    int leavingBlockTargetY;

    GameObjectsManager* objectsManager;
    std::vector<GameObject*>* objectsList;
};

#endif
