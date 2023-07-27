#ifndef BLOCK_BUMP_KILL_ZONE
#define BLOCK_BUMP_KILL_ZONE

#include <SDL.h>

#include "GameObject.h"

class BlockBump : public GameObject {

public:

    BlockBump();

    GameObjectType getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

    void setPosition(SDL_Point* position);

private:

    SDL_Rect hitbox;
};

#endif
