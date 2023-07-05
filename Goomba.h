#ifndef GOOMBA_H
#define GOOMBA_H

#include <SDL.h>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"

class Goomba : public GameObject {

public:

    Goomba(SDL_Renderer* renderer, Level* currentLevel, SDL_FPoint* position);
    ~Goomba();

    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);


private:

    SDL_FPoint position;
    SDL_FPoint velocity;

    SDL_Texture* spriteSheet;
    Animator* walkingAnimator;
    Animator* dyingAnimator;

    Animator* currentAnimator;

    SDL_Point* downCollisionChecks;
    int downCollisionChecksCount;
    SDL_Point* leftCollisionChecks;
    int leftCollisionChecksCount;

    Level* currentLevel;
};

#endif
