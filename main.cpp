#include <SDL.h>
#include <iostream>
#include <SDL_image.h>

#include "GameConfig.h"
#include "Level.h"
#include "GameObjectsManager.h"
#include "Player.h"
#include "Goomba.h"
#include "Input.h"
#include "Time.h"
#include "SpriteSheetRepository.h"
#include "LevelAnimator.h"

int main(int argc, char *argv[]) {

    GameConfig *gameConfig = GameConfig::getInstance();
    int windowWidth = gameConfig->getWindowWidth();
    int windowHeight = gameConfig->getWindowHeight();
    int renderWidth = gameConfig->getRenderWidth();
    int renderHeight = gameConfig->getRenderHeight();

    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Mario!",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
    );
    if (renderer == NULL) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int imgInitFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgInitFlags) & imgInitFlags) != imgInitFlags) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *renderTexture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, renderWidth, renderHeight
    );
    if (!renderTexture) {
        std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return 1;
    }

    SpriteSheetRepository* spriteSheetRepository = SpriteSheetRepository::getInstance();
    if (!spriteSheetRepository->initialize(renderer)) {
        SDL_DestroyTexture(renderTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    Level *level = new Level();
    GameObjectsManager objectsManager;
    LevelAnimator levelAnimator(level, &objectsManager);

    SDL_Point worldCameraPosition = {0, 0};

    SDL_FPoint initialPlayerPosition = {2 * 16, 9 * 16};
    Player* player = new Player(level, &initialPlayerPosition, &objectsManager, &levelAnimator);
    objectsManager.add(player);

    SDL_FPoint initialGoombaPosition = {13 * 16, 9 * 16};
    objectsManager.add(new Goomba(level, &initialGoombaPosition, &objectsManager));

    Time::initialize(60);
    Input* input = Input::getInstance();

    SDL_Event event;
    bool shouldQuit = false;
    while (!shouldQuit) {
        Time::signalFrameStart();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                shouldQuit = true;
            }
        }

        input->update();

        objectsManager.enableObjectsNearCamera(&worldCameraPosition);
        objectsManager.updateAll(&worldCameraPosition);
        objectsManager.processPendingAdditions();

        level->constrainCameraToLevel(&worldCameraPosition);

        SDL_SetRenderTarget(renderer, renderTexture);
        level->clearWithBackgroundColor(renderer);
        level->renderBackgroundTiles(renderer, &worldCameraPosition);
        objectsManager.drawObjects(renderer, &worldCameraPosition);
        level->renderForegroundTiles(renderer, &worldCameraPosition);
        objectsManager.drawTopmostObjects(renderer, &worldCameraPosition);
        levelAnimator.animate(renderer, &worldCameraPosition);

        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, renderTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        objectsManager.destroyNonPlayerObjectsOutsideOfLevel(level);
        objectsManager.destroyNonPlayerObjectsBehindCamera(&worldCameraPosition);
        objectsManager.cleanupDestroyedObjects();

        Time::waitUntilFrameEnd();
    }

    delete level;

    spriteSheetRepository->cleanup();
    SDL_DestroyTexture(renderTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
