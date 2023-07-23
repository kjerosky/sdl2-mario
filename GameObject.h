#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>

enum GameObjectType {
    PLAYER,
    ENEMY,
    FIREBALL,
    POWERUP,
    DECORATION,
};

enum CollisionResponse {
    NO_PROBLEM,
    REVERSE_COURSE,
    GET_STOMPED,
    REACT_TO_STOMP,
    TAKE_DAMAGE,
};

class GameObject {

public:

    GameObject() {
        enabled = false;
    }

    virtual ~GameObject() {
        // nothing to do here - just need a virtual destructor
    }

    SDL_FPoint* getPosition() { return &position; }
    SDL_FPoint* getVelocity() { return &velocity; }
    bool isEnabled() { return enabled; }
    void enable() { enabled = true; }

    virtual GameObjectType getType() = 0;
    virtual SDL_Rect* getHitBox() = 0;
    virtual bool isCollidable() = 0;
    virtual bool isStompable() = 0;
    virtual bool isDrawnOnTop() = 0;
    virtual CollisionResponse receiveCollision(GameObject* sourceObject) = 0;
    virtual void update(SDL_Point *cameraPosition) = 0;
    virtual void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) = 0;

protected:

    SDL_FPoint position;
    SDL_FPoint velocity;
    bool enabled;
};

#endif
