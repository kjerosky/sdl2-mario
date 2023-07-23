#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <SDL.h>

class SpriteSheet {

public:

    SpriteSheet(const char* filePath, int spriteWidth, int spriteHeight, SDL_Renderer* renderer);
    ~SpriteSheet();

    bool wasLoadedSuccessfully();
    void drawSprite(SDL_Renderer* renderer, int spriteIndex, SDL_Point* position);
    void drawSprite(SDL_Renderer* renderer, int spriteIndex, SDL_Point* position, bool flipHorizontally, bool flipVertically);
    int getSpriteCount();

private:

    bool loadedSuccessfully;
    SDL_Texture* texture;
    int spriteWidth;
    int spriteHeight;
    int rows;
    int columns;
};

#endif
