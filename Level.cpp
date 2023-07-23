#include "Level.h"

#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameConfig.h"
#include "SpriteSheetRepository.h"
#include "TilesetConstants.h"

Level::Level() {
    static int staticTileData[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 5, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2, 3, 2, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 5, 5, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
    tileDataCount = sizeof(staticTileData) / sizeof(int);
    tileData = new int[tileDataCount];
    memcpy(tileData, staticTileData, sizeof(staticTileData));

    horizontalTileCount = 40;
    verticalTileCount = 12;

    tileHorizontalPixels = 16;
    tileVerticalPixels = 16;
    levelTiles = SpriteSheetRepository::getInstance()->getSpriteSheetById(TEST_LEVEL_TILES_SHEET);

    backgroundTileIds.insert(0);

    foregroundTileIds.insert(1);
    foregroundTileIds.insert(2);
    foregroundTileIds.insert(3);
    foregroundTileIds.insert(4);
    foregroundTileIds.insert(5);

    // The last sprite in the sheet represents a visually transparent but physically solid tile,
    // mainly used for animating the player bumping a block from below.
    foregroundTileIds.insert(levelTiles->getSpriteCount() - 1);

    backgroundColor.r = 49;
    backgroundColor.g = 171;
    backgroundColor.b = 232;
    backgroundColor.a = 255;
}

Level::~Level() {
    delete[] tileData;
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

            SDL_Point position = {
                x * tileHorizontalPixels - worldCameraPosition->x,
                y * tileVerticalPixels - worldCameraPosition->y,
            };

            levelTiles->drawSprite(renderer, tileId, &position);
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

bool Level::getTileData(SDL_Point *worldPosition, TileInfo* tileInfo) {
    if (worldPosition->x < 0 || worldPosition->y < 0) {
        return false;
    }

    int tileX = worldPosition->x / tileHorizontalPixels;
    int tileY = worldPosition->y / tileVerticalPixels;
    if (tileX >= horizontalTileCount || tileY >= verticalTileCount) {
        return false;
    }

    int tileWorldPositionX = tileX * tileHorizontalPixels;
    int tileWorldPositionY = tileY * tileVerticalPixels;
    int tileDataIndex = tileY * horizontalTileCount + tileX;
    int tileId = tileData[tileDataIndex];
    bool isBrick = tileId == TilesetConstants::BRICK_TILE_ID;
    bool isQuestionBlock = tileId == TilesetConstants::QUESTION_BLOCK_TILE_ID;

    tileInfo->worldPositionX = tileWorldPositionX;
    tileInfo->worldPositionY = tileWorldPositionY;
    tileInfo->tileDataIndex = tileDataIndex;
    tileInfo->tileId = tileId;
    tileInfo->isBrick = isBrick;
    tileInfo->isQuestionBlock = isQuestionBlock;
    return true;
}

void Level::modifyTileData(int tileDataIndex, int tileId) {
    tileData[tileDataIndex] = tileId;
}

SpriteSheet* Level::getLevelTiles() {
    return levelTiles;
}
