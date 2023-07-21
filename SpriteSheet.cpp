#include "SpriteSheet.h"

#include <SDL_image.h>
#include <iostream>

SpriteSheet::SpriteSheet(const char *filePath, int spriteWidth, int spriteHeight, SDL_Renderer* renderer) {
    this->spriteWidth = spriteWidth;
    this->spriteHeight = spriteHeight;

    loadedSuccessfully = false;
    texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        std::cerr << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
        return;
    }

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
    if (textureWidth % spriteWidth != 0) {
        std::cerr << "Sprite sheet \"" << filePath << "\" has width " << textureWidth <<
            ", of which the sprite width " << spriteWidth << " is not an integer multiple" << std::endl;
        return;
    } else if (textureHeight % spriteHeight != 0) {
        std::cerr << "Sprite sheet \"" << filePath << "\" has height " << textureHeight <<
            ", of which the sprite height " << spriteHeight << " is not an integer multiple" << std::endl;
        return;
    }

    rows = textureWidth / spriteWidth;
    columns = textureHeight / spriteHeight;

    loadedSuccessfully = true;
}

SpriteSheet::~SpriteSheet() {
    SDL_DestroyTexture(texture);
}

bool SpriteSheet::wasLoadedSuccessfully() {
    return loadedSuccessfully;
}

void SpriteSheet::drawSprite(SDL_Renderer *renderer, int spriteIndex, SDL_Point *position) {
    drawSprite(renderer, spriteIndex, position, false, false);
}

void SpriteSheet::drawSprite(SDL_Renderer* renderer, int spriteIndex, SDL_Point *position, bool flipHorizontally, bool flipVertically) {
    int targetRow = spriteIndex / rows;
    int targetColumn = spriteIndex % rows;
    int textureX = targetColumn * spriteWidth;
    int textureY = targetRow * spriteHeight;

    SDL_Rect sourceRectangle = {
        textureX,
        textureY,
        spriteWidth,
        spriteHeight
    };

    SDL_Rect destinationRectangle = {
        position->x,
        position->y,
        spriteWidth,
        spriteHeight
    };

    int horizontalFlipping = flipHorizontally ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int verticalFlipping = flipVertically ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
    SDL_RendererFlip flipping = (SDL_RendererFlip)(horizontalFlipping | verticalFlipping);

    if (flipping == SDL_FLIP_NONE) {
        SDL_RenderCopy(renderer, texture, &sourceRectangle, &destinationRectangle);
    } else {
        SDL_RenderCopyEx(renderer, texture, &sourceRectangle, &destinationRectangle, 0, NULL, flipping);
    }
}
