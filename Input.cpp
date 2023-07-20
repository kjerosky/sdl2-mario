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
    fireAndRunScancode = SDL_SCANCODE_J;

    leftHeld = false;
    rightHeld = false;
    upHeld = false;
    downHeld = false;
    jumpPressed = false;
    jumpReleased = false;
    firePressed = false;
    runHeld = false;

    previousJumpState = 0;
    previousFireAndRunState = 0;

    DEBUG_plus_scancode = SDL_SCANCODE_EQUALS;
    DEBUG_plus_buttonPressed = false;
    previous_DEBUG_plus_buttonState = 0;
    DEBUG_minus_scancode = SDL_SCANCODE_MINUS;
    DEBUG_minus_buttonPressed = false;
    previous_DEBUG_minus_buttonState = 0;
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

    Uint8 currentFireAndRunState = keyboardState[fireAndRunScancode];
    firePressed = !previousFireAndRunState && currentFireAndRunState;
    runHeld = currentFireAndRunState;
    previousFireAndRunState = currentFireAndRunState;

    Uint8 current_DEBUG_plus_buttonState = keyboardState[DEBUG_plus_scancode];
    DEBUG_plus_buttonPressed = !previous_DEBUG_plus_buttonState && current_DEBUG_plus_buttonState;
    previous_DEBUG_plus_buttonState = current_DEBUG_plus_buttonState;
    Uint8 current_DEBUG_minus_buttonState = keyboardState[DEBUG_minus_scancode];
    DEBUG_minus_buttonPressed = !previous_DEBUG_minus_buttonState && current_DEBUG_minus_buttonState;
    previous_DEBUG_minus_buttonState = current_DEBUG_minus_buttonState;
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

bool Input::fireWasPressed() {
    return firePressed;
}

bool Input::runIsHeld() {
    return runHeld;
}

bool Input::DEBUG_plus_buttonWasPressed() {
    return DEBUG_plus_buttonPressed;
}

bool Input::DEBUG_minus_buttonWasPressed() {
    return DEBUG_minus_buttonPressed;
}
