#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <iostream>

class GameConfig {

public:

    static GameConfig* getInstance();

    int getWindowWidth();
    int getWindowHeight();
    int getRenderWidth();
    int getRenderHeight();

private:

    GameConfig();
    static GameConfig *instance;

    int windowWidth;
    int windowHeight;

    int renderWidth;
    int renderHeight;
};

#endif
