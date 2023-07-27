#include "LevelAnimator.h"
#include "TilesetConstants.h"
#include "BrickPiece.h"
#include "MagicMushroom.h"
#include "FireFlower.h"

// I got these from recording SMB1 gameplay and analyzing block movements lol
const int LevelAnimator::BLOCK_BUMP_Y_OFFSETS[] = {
    -1, -3, -5, -6, -6, -7, -7, -7, -6, -5, -4, -3, -1, 1
};
const int LevelAnimator::BLOCK_BUMP_FRAMES = sizeof(LevelAnimator::BLOCK_BUMP_Y_OFFSETS) / sizeof(int);
const int LevelAnimator::BRICK_PIECE_SPACING = 8;
const int LevelAnimator::BRICK_PIECE_OFFSET_Y = -4;
const int LevelAnimator::DAMAGING_BUMP_FRAME_INDEX = 6;
const int LevelAnimator::BLOCK_SPACING = 16;

LevelAnimator::LevelAnimator(Level* level, GameObjectsManager* objectsManager) {
    this->level = level;
    this->objectsManager = objectsManager;

    collisionSystem = CollisionSystem::getInstance();

    blockBumpFramesLeft = 0;
    frameCount = 0;
}

void LevelAnimator::animate(SDL_Renderer* renderer, SDL_Point* worldCameraPosition) {
    animateQuestionBlocks();
    frameCount++;

    if (blockBumpFramesLeft <= 0) {
        return;
    }

    SpriteSheet* levelTiles = level->getLevelTiles();

    blockBumpFramesLeft--;
    if (blockBumpFramesLeft == 0) {
        // manually draw the tile for one frame since level rendering runs before this method
        SDL_Point blockCameraPosition = {
            blockPosition.x - worldCameraPosition->x,
            blockPosition.y - worldCameraPosition->y,
        };
        levelTiles->drawSprite(renderer, postBlockBumpTileId, &blockCameraPosition);
        level->modifyTileData(blockBumpTileDataIndex, postBlockBumpTileId);

        if (bumpedBlockContainsPowerup) {
            spawnPowerup(blockPosition.x, blockPosition.y);
        }
    } else {
        int blockBumpFrameIndex = BLOCK_BUMP_FRAMES - blockBumpFramesLeft;
        SDL_Point offsetBlockPosition = {
            blockPosition.x - worldCameraPosition->x,
            blockPosition.y - worldCameraPosition->y + BLOCK_BUMP_Y_OFFSETS[blockBumpFrameIndex],
        };
        levelTiles->drawSprite(renderer, postBlockBumpTileId, &offsetBlockPosition);

        if (blockBumpFrameIndex == DAMAGING_BUMP_FRAME_INDEX) {
            bumpEnemyAboveBlock();
        }
    }
}

bool LevelAnimator::animatePlayerBonk(SDL_Point* worldPoint, bool playerIsPoweredUp) {
    this->playerIsPoweredUp = playerIsPoweredUp;

    TileInfo tileInfo;
    if (!level->getTileData(worldPoint, &tileInfo)) {
        return false;
    }

    bool wasSoftBonk = false;
    if (tileInfo.isQuestionBlock || (tileInfo.isBrick && tileInfo.containsPowerup)) {
        setupBlockBumpAnimation(
            tileInfo.worldPositionX,
            tileInfo.worldPositionY,
            tileInfo.tileDataIndex,
            TilesetConstants::HIT_QUESTION_BLOCK_TILE_ID,
            tileInfo.containsPowerup
        );
    } else if (tileInfo.isBrick && playerIsPoweredUp) {
        blockPosition.x = tileInfo.worldPositionX;
        blockPosition.y = tileInfo.worldPositionY;
        bumpEnemyAboveBlock();

        level->modifyTileData(tileInfo.tileDataIndex, TilesetConstants::TRANSPARENT_TILE_ID);
        spawnBrickPieces(tileInfo.worldPositionX, tileInfo.worldPositionY);
        wasSoftBonk = true;
    } else if (tileInfo.isBrick) {
        setupBlockBumpAnimation(
            tileInfo.worldPositionX,
            tileInfo.worldPositionY,
            tileInfo.tileDataIndex,
            tileInfo.tileId,
            tileInfo.containsPowerup
        );
    }

    return wasSoftBonk;
}

void LevelAnimator::setupBlockBumpAnimation(int worldPositionX, int worldPositionY, int tileDataIndex, int postAnimationTileId, bool blockContainsPowerup) {
    // ignore block bump if another is being animated
    if (blockBumpFramesLeft > 0) {
        level->modifyTileData(blockBumpTileDataIndex, postBlockBumpTileId);
    }

    blockPosition.x = worldPositionX;
    blockPosition.y = worldPositionY;
    blockBumpTileDataIndex = tileDataIndex;
    postBlockBumpTileId = postAnimationTileId;
    blockBumpFramesLeft = BLOCK_BUMP_FRAMES + 1;
    bumpedBlockContainsPowerup = blockContainsPowerup;

    SpriteSheet* levelTiles = level->getLevelTiles();
    int solidTransparentSpriteIndex = levelTiles->getSpriteCount() - 1;
    level->modifyTileData(tileDataIndex, solidTransparentSpriteIndex);
}

void LevelAnimator::spawnBrickPieces(int worldPositionX, int worldPositionY) {
    SDL_Point position = {
        worldPositionX,
        worldPositionY + BRICK_PIECE_OFFSET_Y
    };
    BrickPiece* upperLeftBrickPiece = new BrickPiece(&position, UP_LEFT);

    position.x += BRICK_PIECE_SPACING;
    BrickPiece* upperRightBrickPiece = new BrickPiece(&position, UP_RIGHT);

    position.y += BRICK_PIECE_SPACING;
    BrickPiece* lowerRightBrickPiece = new BrickPiece(&position, DOWN_RIGHT);

    position.x -= BRICK_PIECE_SPACING;
    BrickPiece* lowerLeftBrickPiece = new BrickPiece(&position, DOWN_LEFT);

    objectsManager->add(upperLeftBrickPiece);
    objectsManager->add(upperRightBrickPiece);
    objectsManager->add(lowerRightBrickPiece);
    objectsManager->add(lowerLeftBrickPiece);
}

void LevelAnimator::spawnPowerup(int blockPositionX, int blockPositionY) {
    SDL_FPoint powerupBlockPosition = {blockPositionX, blockPositionY};
    if (playerIsPoweredUp) {
        objectsManager->add(new FireFlower(level, &powerupBlockPosition, objectsManager));
    } else {
        objectsManager->add(new MagicMushroom(level, &powerupBlockPosition, objectsManager));
    }
}

void LevelAnimator::animateQuestionBlocks() {
    // animation frame counts: 24 (tile 1), 9 (tile 2), 7 (tile 3), 9 (tile 2), ...
    int frame = frameCount % 49;
    if (frame < 24) {
        level->animateTileId(TilesetConstants::QUESTION_BLOCK_TILE_ID, TilesetConstants::QUESTION_BLOCK_TILE_ID);
    } else if (frame >= 24 && frame < 33) {
        level->animateTileId(TilesetConstants::QUESTION_BLOCK_TILE_ID, TilesetConstants::QUESTION_BLOCK_2_TILE_ID);
    } else if (frame >= 33 && frame < 40) {
        level->animateTileId(TilesetConstants::QUESTION_BLOCK_TILE_ID, TilesetConstants::QUESTION_BLOCK_3_TILE_ID);
    } else {
        level->animateTileId(TilesetConstants::QUESTION_BLOCK_TILE_ID, TilesetConstants::QUESTION_BLOCK_2_TILE_ID);
    }
}

void LevelAnimator::bumpEnemyAboveBlock() {
    SDL_Point killZonePosition = {
        blockPosition.x,
        blockPosition.y - BLOCK_SPACING,
    };
    blockBumpKillZone.setPosition(&killZonePosition);
    collisionSystem->testObjectAgainstAllOthers(&blockBumpKillZone);
}
