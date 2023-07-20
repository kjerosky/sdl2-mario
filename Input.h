#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

class Input {

public:

    static Input* getInstance();

    void update();

    bool leftIsHeld();
    bool rightIsHeld();
    bool upIsHeld();
    bool downIsHeld();

    bool jumpWasPressed();
    bool jumpWasReleased();

    bool DEBUG_plus_buttonWasPressed();
    bool DEBUG_minus_buttonWasPressed();

private:

    Input();
    static Input *instance;

    const Uint8 *keyboardState;

    SDL_Scancode leftScancode;
    SDL_Scancode rightScancode;
    SDL_Scancode upScancode;
    SDL_Scancode downScancode;
    SDL_Scancode jumpScancode;

    bool leftHeld;
    bool rightHeld;
    bool upHeld;
    bool downHeld;
    bool jumpPressed;
    bool jumpReleased;

    Uint8 previousJumpState;

    //TODO REMOVE THIS EVENTUALLY
    SDL_Scancode DEBUG_plus_scancode;
    bool DEBUG_plus_buttonPressed;
    Uint8 previous_DEBUG_plus_buttonState;
    SDL_Scancode DEBUG_minus_scancode;
    bool DEBUG_minus_buttonPressed;
    Uint8 previous_DEBUG_minus_buttonState;
};

#endif
