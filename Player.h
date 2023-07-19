#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"
#include "Input.h"
#include "GameObjectsManager.h"

class Player : public GameObject {

public:

    Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, GameObjectsManager* objectsManager);
    ~Player();

    GameObject::Type getType();
    SDL_Rect* getHitBox();
    bool isStompable();
    GameObject::CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    const static float JUMP_VELOCITY;
    const static float BONK_VELOCITY;
    const static float JUMP_GRAVITY;
    const static float FALL_GRAVITY;
    const static float STOMP_REACTION_VELOCITY;

    enum PlayerState {
        ON_GROUND,
        JUMPING,
        FALLING,
    };
    PlayerState state;

    void checkStateTransitions();
    void processCurrentState();

    void updateGroundStatus();
    void applyHorizontalMovement();
    void applyVerticalMovement(float gravity);
    void resolveCollisions();
    void animateSprite();
    void centerCameraOnPlayer(SDL_Point* cameraPosition);

    Input* input;

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

    GameObjectsManager* objectsManager;
    std::vector<GameObject*>* objectsList;
};

#endif
