#include "Player.h"

#include <SDL_image.h>
#include <iostream>
#include "GameConfig.h"

Player::Player(SDL_Renderer *renderer, SDL_Point *position) {
    this->position = *position;

    spriteSheet = IMG_LoadTexture(renderer, "assets/mario.png");
    if (!spriteSheet) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    keyboardState = SDL_GetKeyboardState(NULL);
}

Player::~Player() {
    SDL_DestroyTexture(spriteSheet);
}

void Player::update(SDL_Point *cameraPosition) {
    int renderWidth = GameConfig::getInstance()->getRenderWidth();

    position.x += keyboardState[SDL_SCANCODE_D] - keyboardState[SDL_SCANCODE_A];
    position.y += keyboardState[SDL_SCANCODE_S] - keyboardState[SDL_SCANCODE_W];

    cameraPosition->x = position.x + 16 / 2 - renderWidth / 2;
}

void Player::draw(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    SDL_Rect destinationRectangle = {
        position.x - cameraPosition->x,
        position.y - cameraPosition->y,
        16,
        16
    };
    SDL_RenderCopy(renderer, spriteSheet, NULL, &destinationRectangle);
}
