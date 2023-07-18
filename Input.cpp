#include "Input.h"
#include <iostream>

Input* Input::instance = NULL;

Input* Input::getInstance() {
    if (instance == NULL) {
        instance = new Input();
    }
    return instance;
}

Input::Input() {
    keyboardState = SDL_GetKeyboardState(NULL);

    leftScancode = SDL_SCANCODE_A;
    rightScancode = SDL_SCANCODE_D;
    upScancode = SDL_SCANCODE_W;
    downScancode = SDL_SCANCODE_S;
    jumpScancode = SDL_SCANCODE_K;

    leftHeld = false;
    rightHeld = false;
    upHeld = false;
    downHeld = false;
    jumpPressed = false;
    jumpReleased = false;

    previousJumpState = 0;
}

void Input::update() {
    leftHeld = keyboardState[leftScancode];
    rightHeld = keyboardState[rightScancode];
    upHeld = keyboardState[upScancode];
    downHeld = keyboardState[downScancode];

    Uint8 currentJumpState = keyboardState[jumpScancode];
    jumpPressed = !previousJumpState && currentJumpState;
    jumpReleased = previousJumpState && !currentJumpState;

    previousJumpState = currentJumpState;
}

bool Input::leftIsHeld() {
    return leftHeld;
}

bool Input::rightIsHeld() {
    return rightHeld;
}

bool Input::upIsHeld() {
    return upHeld;
}

bool Input::downIsHeld() {
    return downHeld;
}

bool Input::jumpWasPressed() {
    return jumpPressed;
}

bool Input::jumpWasReleased() {
    return jumpReleased;
}
