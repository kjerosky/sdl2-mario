#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>

class Level {

public:

    Level(SDL_Renderer *renderer, const char *levelTilesFilename);
    ~Level();

    void render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);
    void constrainCameraToLevel(SDL_Point *cameraPosition);

private:

    int *tileData;
    int tileDataCount;
    int horizontalTileCount;
    int verticalTileCount;

    SDL_Texture *levelTiles;
    int tileHorizontalPixels;
    int tileVerticalPixels;
    SDL_Color backgroundColor;
};

#endif
