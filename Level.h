#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <stdint.h>

class Level {

public:

    Level(SDL_Renderer *renderer, const char *levelTilesFilename);
    ~Level();

    void render(SDL_Renderer *renderer, SDL_Point *worldCameraPosition);

private:

    SDL_Texture *levelTiles;
    int *tileData;
    SDL_Color backgroundColor;

};

#endif
