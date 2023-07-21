#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>

class GameObject {

public:

    enum Type {
        PLAYER,
        ENEMY,
        FIREBALL,
        POWERUP
    };

    enum CollisionResponse {
        NO_PROBLEM,
        REVERSE_COURSE,
        GET_STOMPED,
        REACT_TO_STOMP,
        TAKE_DAMAGE
    };

    virtual ~GameObject() {
        // nothing to do here - just need a virtual destructor
    }

    SDL_FPoint* getPosition() { return &position; }
    SDL_FPoint* getVelocity() { return &velocity; }

    virtual GameObject::Type getType() = 0;
    virtual SDL_Rect* getHitBox() = 0;
    virtual bool isStompable() = 0;
    virtual bool isDrawnOnTop() = 0;
    virtual GameObject::CollisionResponse receiveCollision(GameObject* sourceObject) = 0;
    virtual void update(SDL_Point *cameraPosition) = 0;
    virtual void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) = 0;

protected:

    SDL_FPoint position;
    SDL_FPoint velocity;
};

#endif
