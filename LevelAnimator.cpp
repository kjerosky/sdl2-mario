#include "LevelAnimator.h"
#include "TilesetConstants.h"

// I got these from recording SMB1 gameplay and analyzing block movements lol
const int LevelAnimator::BLOCK_BUMP_Y_OFFSETS[] = {
    -1, -3, -5, -6, -6, -7, -7, -7, -6, -5, -4, -3, -1, 1
};
const int LevelAnimator::BLOCK_BUMP_FRAMES = sizeof(LevelAnimator::BLOCK_BUMP_Y_OFFSETS) / sizeof(int);

LevelAnimator::LevelAnimator(Level* level) {
    this->level = level;

    blockBumpFramesLeft = 0;
}

void LevelAnimator::animate(SDL_Renderer* renderer, SDL_Point* worldCameraPosition) {
    if (blockBumpFramesLeft <= 0) {
        return;
    }

    SpriteSheet* levelTiles = level->getLevelTiles();

    blockBumpFramesLeft--;
    if (blockBumpFramesLeft == 0) {
        // manually draw the tile for one frame since level rendering runs before this method
        levelTiles->drawSprite(renderer, postBlockBumpTileId, &blockPosition);
        level->modifyTileData(blockBumpTileDataIndex, postBlockBumpTileId);
    } else {

        SDL_Point offsetBlockPosition = {
            blockPosition.x - worldCameraPosition->x,
            blockPosition.y - worldCameraPosition->y + BLOCK_BUMP_Y_OFFSETS[BLOCK_BUMP_FRAMES - blockBumpFramesLeft],
        };
        levelTiles->drawSprite(renderer, postBlockBumpTileId, &offsetBlockPosition);
    }
}

bool LevelAnimator::animatePlayerBonk(SDL_Point *worldPoint, bool isPoweredUp) {
    TileInfo tileInfo;
    if (!level->getTileData(worldPoint, &tileInfo)) {
        return false;
    }

    bool wasSoftBonk = false;
    if (tileInfo.isQuestionBlock) {
        setupBlockBumpAnimation(
            tileInfo.worldPositionX,
            tileInfo.worldPositionY,
            tileInfo.tileDataIndex,
            TilesetConstants::HIT_QUESTION_BLOCK_TILE_ID
        );
    } else if (tileInfo.isBrick) {
        if (isPoweredUp) {
            level->modifyTileData(tileInfo.tileDataIndex, TilesetConstants::TRANSPARENT_TILE_ID);
            //TODO SPAWN BRICK PIECES HERE
            wasSoftBonk = true;
        } else {
            setupBlockBumpAnimation(
                tileInfo.worldPositionX,
                tileInfo.worldPositionY,
                tileInfo.tileDataIndex,
                tileInfo.tileId
            );
        }
    }

    return wasSoftBonk;
}

void LevelAnimator::setupBlockBumpAnimation(int worldPositionX, int worldPositionY, int tileDataIndex, int postAnimationTileId) {
    // flush current bonk animation
    if (blockBumpFramesLeft > 0) {
        level->modifyTileData(blockBumpTileDataIndex, postBlockBumpTileId);
    }

    blockPosition.x = worldPositionX;
    blockPosition.y = worldPositionY;
    blockBumpTileDataIndex = tileDataIndex;
    postBlockBumpTileId = postAnimationTileId;
    blockBumpFramesLeft = BLOCK_BUMP_FRAMES + 1;

    SpriteSheet* levelTiles = level->getLevelTiles();
    int solidTransparentSpriteIndex = levelTiles->getSpriteCount() - 1;
    level->modifyTileData(tileDataIndex, solidTransparentSpriteIndex);
}
