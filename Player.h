#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"
#include "Animator.h"
#include "Input.h"
#include "GameObjectsManager.h"
#include "SpriteSheet.h"

class Player : public GameObject {

public:

    Player(SDL_Renderer *renderer, Level *currentLevel, SDL_FPoint *position, GameObjectsManager* objectsManager);
    ~Player();

    GameObject::Type getType();
    SDL_Rect* getHitBox();
    bool isCollidable();
    bool isStompable();
    bool isDrawnOnTop();
    GameObject::CollisionResponse receiveCollision(GameObject* sourceObject);
    void update(SDL_Point *cameraPosition);
    void draw(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    const static float JUMP_VELOCITY;
    const static float BONK_VELOCITY;
    const static float JUMP_GRAVITY;
    const static float FALL_GRAVITY;
    const static float STOMP_REACTION_VELOCITY;
    const static int SPRITE_WIDTH;
    const static int THROW_FIREBALL_FRAME_COUNT;
    const static int INVINCIBILITY_FRAMES_COUNT;

    enum PlayerState {
        ON_GROUND,
        JUMPING,
        FALLING,
    };
    PlayerState state;

    enum PowerState {
        SMALL_MARIO,
        SUPER_MARIO,
        FIRE_MARIO,
    };
    PowerState powerState;

    void checkStateTransitions();
    void processCurrentState();

    void updateGroundStatus();
    void applyHorizontalMovement();
    void applyVerticalMovement(float gravity);
    void resolveCollisions();
    void animateSprite();
    void centerCameraOnPlayer(SDL_Point* cameraPosition);
    void attemptFireballThrow();

    void takeDamage();
    void powerUp();
    void powerDown();

    Input* input;

    //TODO REMOVE THIS WHEN SPRITE SHEETS ARE CENTRALIZED
    SDL_Renderer* fireballRenderer;

    bool facingRight;
    bool isGrounded;

    SpriteSheet* smallMarioSpriteSheet;
    Animator* smallMarioStandingAnimator;
    Animator* smallMarioWalkingAnimator;
    Animator* smallMarioJumpingAnimator;

    SpriteSheet* superMarioSpriteSheet;
    Animator* superMarioStandingAnimator;
    Animator* superMarioWalkingAnimator;
    Animator* superMarioJumpingAnimator;

    SpriteSheet* fireMarioSpriteSheet;
    Animator* fireMarioStandingAnimator;
    Animator* fireMarioWalkingAnimator;
    Animator* fireMarioJumpingAnimator;
    Animator* fireMarioThrowingFireballAnimator;

    Animator* currentAnimator;

    SDL_Point *smallSizeDownCollisionChecks;
    int smallSizeDownCollisionChecksCount;
    SDL_Point *smallSizeRightCollisionChecks;
    int smallSizeRightCollisionChecksCount;

    SDL_Point *poweredUpDownCollisionChecks;
    int poweredUpDownCollisionChecksCount;
    SDL_Point *poweredUpRightCollisionChecks;
    int poweredUpRightCollisionChecksCount;

    int currentDownCollisionChecksCount;
    SDL_Point** currentDownCollisionChecks;
    int currentRightCollisionChecksCount;
    SDL_Point** currentRightCollisionChecks;

    SDL_Rect smallMarioHitBox;
    SDL_Rect poweredUpMarioHitBox;

    SDL_Rect* currentHitBox;

    int currentSpriteHeight;

    int throwFireballFramesLeft;

    Level *currentLevel;

    GameObjectsManager* objectsManager;
    std::vector<GameObject*>* objectsList;

    int invincibilityFramesLeft;
};

#endif
