#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"

class Player : public GameObject {

public:

    Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, std::vector<GameObject*>* objectsList);
    ~Player();

    GameObject::Type getType();
    SDL_Rect* getHitBox();
    bool isStompable();
    GameObject::CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    bool facingRight;
    bool isGrounded;

    SDL_Texture* smallMarioSpriteSheet;
    Animator* smallMarioStandingAnimator;
    Animator* smallMarioWalkingAnimator;
    Animator* smallMarioJumpingAnimator;

    Animator* currentAnimator;

    SDL_Point *smallSizeDownCollisionChecks;
    int smallSizeDownCollisionChecksCount;
    SDL_Point *smallSizeRightCollisionChecks;
    int smallSizeRightCollisionChecksCount;

    SDL_Rect smallMarioHitBox;

    SDL_Rect* currentHitBox;

    Level *currentLevel;

    std::vector<GameObject*>* objectsList;
};

#endif
