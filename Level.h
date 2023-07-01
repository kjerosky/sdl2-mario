#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <set>

class Level {

public:

    Level(SDL_Renderer *renderer, const char *levelTilesFilename);
    ~Level();

    void constrainCameraToLevel(SDL_Point *cameraPosition);
    void clearWithBackgroundColor(SDL_Renderer *renderer);
    void renderBackgroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);
    void renderForegroundTiles(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);

private:

    int *tileData;
    int tileDataCount;
    int horizontalTileCount;
    int verticalTileCount;

    SDL_Texture *levelTiles;
    int tileHorizontalPixels;
    int tileVerticalPixels;
    SDL_Color backgroundColor;
    std::set<int> foregroundTileIds;
    std::set<int> backgroundTileIds;

    void render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition, std::set<int> *renderForeground);
};

#endif
