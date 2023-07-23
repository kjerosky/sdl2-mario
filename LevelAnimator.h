#ifndef LEVEL_ANIMATOR_H
#define LEVEL_ANIMATOR_H

#include <SDL.h>

#include "Level.h"

class LevelAnimator {

public:

    LevelAnimator(Level* level);

    void animate(SDL_Renderer* renderer, SDL_Point* worldCameraPosition);
    bool animatePlayerBonk(SDL_Point* worldPoint, bool isPoweredUp);

private:

    const static int BLOCK_BUMP_Y_OFFSETS[];
    const static int BLOCK_BUMP_FRAMES;

    void setupBlockBumpAnimation(int worldPositionX, int worldPositionY, int tileDataIndex, int postAnimationTileId);

    Level* level;

    int blockBumpTileDataIndex;
    int postBlockBumpTileId;
    int blockBumpFramesLeft;
    SDL_Point blockPosition;
};

#endif
