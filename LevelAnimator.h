#ifndef LEVEL_ANIMATOR_H
#define LEVEL_ANIMATOR_H

#include <SDL.h>

#include "Level.h"
#include "GameObjectsManager.h"
#include "GameObject.h"
#include "CollisionSystem.h"
#include "BlockBumpKillZone.h"

class LevelAnimator {

public:

    LevelAnimator(Level* level, GameObjectsManager* objectsManager);

    void animate(SDL_Renderer* renderer, SDL_Point* worldCameraPosition);
    bool animatePlayerBonk(SDL_Point* worldPoint, bool playerIsPoweredUp);

private:

    const static int BLOCK_BUMP_Y_OFFSETS[];
    const static int BLOCK_BUMP_FRAMES;
    const static int BRICK_PIECE_SPACING;
    const static int BRICK_PIECE_OFFSET_Y;
    const static int DAMAGING_BUMP_FRAME_INDEX;
    const static int BLOCK_SPACING;

    void setupBlockBumpAnimation(int worldPositionX, int worldPositionY, int tileDataIndex, int postAnimationTileId, bool blockContainsPowerup);
    void spawnBrickPieces(int worldPositionX, int worldPositionY);
    void spawnPowerup(int blockPositionX, int blockPositionY);
    void animateQuestionBlocks();
    void bumpEnemyAboveBlock();

    Level* level;
    GameObjectsManager* objectsManager;

    int blockBumpTileDataIndex;
    int postBlockBumpTileId;
    int blockBumpFramesLeft;
    SDL_Point blockPosition;
    bool bumpedBlockContainsPowerup;

    bool playerIsPoweredUp;

    int frameCount;

    CollisionSystem* collisionSystem;
    BlockBump blockBumpKillZone;
};

#endif
