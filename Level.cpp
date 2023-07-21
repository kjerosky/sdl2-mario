#include "Level.h"

#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameConfig.h"

Level::Level(SDL_Renderer *renderer, const char *levelTilesFilename) {
    static int staticTileData[] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    tileDataCount = sizeof(staticTileData) / sizeof(int);
    tileData = new int[tileDataCount];
    memcpy(tileData, staticTileData, sizeof(staticTileData));

    horizontalTileCount = 40;
    verticalTileCount = 12;

    levelTiles = IMG_LoadTexture(renderer, levelTilesFilename);
    if (!levelTiles) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
    }

    backgroundTileIds.insert(0);
    backgroundTileIds.insert(2);

    foregroundTileIds.insert(1);

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

void Level::clearWithBackgroundColor(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(
        renderer,
        backgroundColor.r,
        backgroundColor.g,
        backgroundColor.b,
        backgroundColor.a
    );
    SDL_RenderClear(renderer);
}

void Level::renderBackgroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition) {
    render(renderer, worldCameraPosition, &backgroundTileIds);
}

void Level::renderForegroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition) {
    render(renderer, worldCameraPosition, &foregroundTileIds);
}

void Level::render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition, std::set<int> *tileIdsToRender) {
    GameConfig *gameConfig = GameConfig::getInstance();
    int renderWidth = gameConfig->getRenderWidth();
    int renderHeight = gameConfig->getRenderHeight();

    int visibleHorizontalTiles = ceil(renderWidth / (float)tileHorizontalPixels);
    int visibleVerticalTiles = ceil(renderHeight / (float)tileVerticalPixels);

    int leftMostTileX = std::max(worldCameraPosition->x / tileHorizontalPixels, 0);
    int rightMostTileX = std::min(leftMostTileX + visibleHorizontalTiles, horizontalTileCount - 1);
    int topMostTileY = std::max(worldCameraPosition->y / tileVerticalPixels, 0);
    int bottomMostTileY = std::min(topMostTileY + visibleVerticalTiles, verticalTileCount - 1);

    SDL_Rect sourceRect = {0, 0, tileHorizontalPixels, tileVerticalPixels};
    for (int y = topMostTileY; y <= bottomMostTileY; y++) {
        for (int x = leftMostTileX; x <= rightMostTileX; x++) {
            int tileDataIndex = y * horizontalTileCount + x;
            if (tileDataIndex >= tileDataCount) {
                continue;
            }

            int tileId = tileData[tileDataIndex];
            if (tileIdsToRender->find(tileId) == tileIdsToRender->end()) {
                continue;
            }

            sourceRect.x = tileId * tileHorizontalPixels;

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

void Level::constrainCameraToLevel(SDL_Point *cameraPosition) {
    GameConfig *gameConfig = GameConfig::getInstance();
    int renderWidth = gameConfig->getRenderWidth();
    int renderHeight = gameConfig->getRenderHeight();

    int maxAllowedCameraX = horizontalTileCount * tileHorizontalPixels - renderWidth;
    int maxAllowedCameraY = verticalTileCount * tileVerticalPixels - renderHeight;

    cameraPosition->x = SDL_clamp(cameraPosition->x, 0, maxAllowedCameraX);
    cameraPosition->y = SDL_clamp(cameraPosition->y, 0, maxAllowedCameraY);
}

bool Level::isWorldPositionInForegroundTile(SDL_Point *worldPosition) {
    if (worldPosition->x < 0 || worldPosition->y < 0) {
        return false;
    }

    int tileX = worldPosition->x / tileHorizontalPixels;
    int tileY = worldPosition->y / tileVerticalPixels;
    if (tileX >= horizontalTileCount || tileY >= verticalTileCount) {
        return false;
    }

    int tileDataIndex = tileY * horizontalTileCount + tileX;
    int tileId = tileData[tileDataIndex];
    return foregroundTileIds.find(tileId) != foregroundTileIds.end();
}

bool Level::isWorldPositionOutsideLevel(SDL_Point *worldPosition) {
    return
        worldPosition->x >= horizontalTileCount * tileHorizontalPixels ||
        worldPosition->y >= verticalTileCount * tileVerticalPixels
    ;
}
