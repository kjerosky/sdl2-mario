#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include <SDL.h>

class Player : public GameObject {

public:

    Player(SDL_Renderer *renderer, SDL_Point *position);
    ~Player();

    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    SDL_Point position;
    SDL_Texture* spriteSheet;

    const Uint8* keyboardState;
};

#endif
