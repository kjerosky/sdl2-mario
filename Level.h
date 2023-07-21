#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <set>

#include "SpriteSheet.h"

class Level {

public:

    Level();
    ~Level();

    void constrainCameraToLevel(SDL_Point *cameraPosition);
    void clearWithBackgroundColor(SDL_Renderer *renderer);
    void renderBackgroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);
    void renderForegroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);
    bool isWorldPositionInForegroundTile(SDL_Point *worldPosition);
    bool isWorldPositionOutsideLevel(SDL_Point *worldPosition);

private:

    int *tileData;
    int tileDataCount;
    int horizontalTileCount;
    int verticalTileCount;

    SpriteSheet* levelTiles;
    int tileHorizontalPixels;
    int tileVerticalPixels;
    SDL_Color backgroundColor;
    std::set<int> foregroundTileIds;
    std::set<int> backgroundTileIds;

    void render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition, std::set<int> *renderForeground);
};

#endif
