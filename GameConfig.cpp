#include "GameConfig.h"

GameConfig* GameConfig::instance = NULL;

GameConfig* GameConfig::getInstance() {
    if (instance == NULL) {
        instance = new GameConfig();
    }
    return instance;
}

GameConfig::GameConfig() {
    windowWidth = 1280;
    windowHeight = 720;

    renderWidth = 320;
    renderHeight = 180;
}

int GameConfig::getWindowWidth() {
    return windowWidth;
}

int GameConfig::getWindowHeight() {
    return windowHeight;
}

int GameConfig::getRenderWidth() {
    return renderWidth;
}

int GameConfig::getRenderHeight() {
    return renderHeight;
}
