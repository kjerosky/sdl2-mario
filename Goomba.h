#ifndef GOOMBA_H
#define GOOMBA_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"

class Goomba : public GameObject {

public:

    Goomba(SDL_Renderer* renderer, Level* currentLevel, SDL_FPoint* position, std::vector<GameObject*>* objectsList);
    ~Goomba();

    GameObject::Type getType();
    SDL_Rect* getHitBox();
    bool isStompable();
    GameObject::CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    enum GoombaState {
        WALKING,
        STOMPED,
        DEAD
    };

    const static Uint64 STOMPED_TIME = 1000;

    Uint64 stompedTimer;

    SDL_Texture* spriteSheet;
    Animator* walkingAnimator;
    Animator* stompedAnimator;

    Animator* currentAnimator;

    SDL_Point* downCollisionChecks;
    int downCollisionChecksCount;
    SDL_Point* leftCollisionChecks;
    int leftCollisionChecksCount;

    SDL_Rect hitBox;

    Level* currentLevel;

    std::vector<GameObject*>* objectsList;

    GoombaState state;
};

#endif
