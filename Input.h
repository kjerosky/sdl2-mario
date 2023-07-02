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

private:

    Input();
    static Input *instance;

    const Uint8 *keyboardState;

    SDL_Scancode leftScancode;
    SDL_Scancode rightScancode;
    SDL_Scancode upScancode;
    SDL_Scancode downScancode;

    bool leftHeld;
    bool rightHeld;
    bool upHeld;
    bool downHeld;
};

#endif
