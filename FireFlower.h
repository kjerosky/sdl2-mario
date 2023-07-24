#ifndef FIRE_FLOWER_H
#define FIRE_FLOWER_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "GameObjectsManager.h"
#include "SpriteSheet.h"
#include "Animator.h"

class FireFlower : public GameObject {

public:

    FireFlower(Level* level, SDL_FPoint* blockPosition, GameObjectsManager* objectsManager);
    ~FireFlower();

    GameObjectType getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point* cameraPosition);
    void draw(SDL_Renderer* renderer, SDL_Point* cameraPosition);

private:

    const static int SPAWN_OFFSET_Y;
    const static int SPRITE_HEIGHT;
    const static float LEAVING_BLOCK_VELOCITY_Y;

    enum FireFlowerState {
        LEAVING_BLOCK,
        STANDING_STILL,
        CONSUMED,
    };
    FireFlowerState state;

    void checkStateTransitions();
    void processCurrentState();
    void getConsumedByPlayer();

    Level* level;

    SDL_Rect hitbox;

    SpriteSheet* spriteSheet;
    Animator* animator;

    int leavingBlockTargetY;

    GameObjectsManager* objectsManager;
};

#endif
