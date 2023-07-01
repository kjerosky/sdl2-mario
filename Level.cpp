#include "Level.h"

#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameConfig.h"

Level::Level(SDL_Renderer *renderer, const char *levelTilesFilename) {
    static int staticTileData[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    tileDataCount = sizeof(staticTileData) / sizeof(int);
    tileData = new int[tileDataCount];
    memcpy(tileData, staticTileData, sizeof(staticTileData));

    horizontalTileCount = 40;
    verticalTileCount = 14;

    levelTiles = IMG_LoadTexture(renderer, levelTilesFilename);
    if (!levelTiles) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    tileHorizontalPixels = 16;
    tileVerticalPixels = 16;

    backgroundColor.r = 49;
    backgroundColor.g = 171;
    backgroundColor.b = 232;
    backgroundColor.a = 255;
}

Level::~Level() {
    delete[] tileData;
    SDL_DestroyTexture(levelTiles);
}

void Level::render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition) {
    GameConfig *gameConfig = GameConfig::getInstance();
    int renderWidth = gameConfig->getRenderWidth();
    int renderHeight = gameConfig->getRenderHeight();

    int visibleHorizontalTiles = ceil(renderWidth / (float)tileHorizontalPixels);
    int visibleVerticalTiles = ceil(renderHeight / (float)tileVerticalPixels);

    int leftMostTileX = std::max(worldCameraPosition->x / tileHorizontalPixels, 0);
    int rightMostTileX = std::min(leftMostTileX + visibleHorizontalTiles, horizontalTileCount - 1);
    int topMostTileY = std::max(worldCameraPosition->y / tileVerticalPixels, 0);
    int bottomMostTileY = std::min(topMostTileY + visibleVerticalTiles, verticalTileCount - 1);

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    SDL_Rect sourceRect = {0, 0, tileHorizontalPixels, tileVerticalPixels};
    for (int y = topMostTileY; y <= bottomMostTileY; y++) {
        for (int x = leftMostTileX; x <= rightMostTileX; x++) {
            int tileDataIndex = y * horizontalTileCount + x;
            if (tileDataIndex >= tileDataCount) {
                continue;
            }

            sourceRect.x = tileData[tileDataIndex] * tileHorizontalPixels;

            SDL_Rect destinationRect = {
                x * tileHorizontalPixels - worldCameraPosition->x,
                y * tileVerticalPixels - worldCameraPosition->y,
                tileHorizontalPixels,
                tileVerticalPixels
            };

            SDL_RenderCopy(renderer, levelTiles, &sourceRect, &destinationRect);
        }
    }
}
