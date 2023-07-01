#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SDL.h>

class GameObject {

public:

    virtual ~GameObject() {
        // nothing to do for an interface
    }

    virtual void update(SDL_Point *cameraPosition) = 0;
    virtual void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) = 0;
};

#endif
