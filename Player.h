#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"

class Player : public GameObject {

public:

    Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position);
    ~Player();

    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    SDL_FPoint position;
    SDL_FPoint velocity;
    bool facingRight;
    bool isGrounded;

    SDL_Texture* smallMarioSpriteSheet;
    Animator* smallMarioStandingAnimator;

    Animator* currentAnimator;

    SDL_Point *smallSizeDownCollisionChecks;
    int smallSizeDownCollisionChecksCount;
    SDL_Point *smallSizeRightCollisionChecks;
    int smallSizeRightCollisionChecksCount;

    Level *currentLevel;
};

#endif
