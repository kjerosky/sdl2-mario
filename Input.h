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

    bool DEBUG_buttonWasPressed();

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
    SDL_Scancode DEBUG_scancode;
    bool DEBUG_buttonPressed;
    Uint8 previous_DEBUG_buttonState;
};

#endif
