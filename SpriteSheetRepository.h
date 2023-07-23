#ifndef SPRITE_SHEET_REPOSITORY_H
#define SPRITE_SHEET_REPOSITORY_H

#include <map>

#include "SpriteSheet.h"

enum SpriteSheetId {
    TEST_LEVEL_TILES_SHEET,
    SMALL_MARIO_SHEET,
    SUPER_MARIO_SHEET,
    FIRE_MARIO_SHEET,
    GOOMBA_SHEET,
    FIREBALL_SHEET,
    BRICK_PIECES_SHEET,
    POWERUPS_SHEET,
};

class SpriteSheetRepository {

public:

    static SpriteSheetRepository* getInstance();

    bool initialize(SDL_Renderer* renderer);
    SpriteSheet* getSpriteSheetById(SpriteSheetId id);
    void cleanup();

private:

    SpriteSheetRepository();
    static SpriteSheetRepository* instance;

    std::map<SpriteSheetId, SpriteSheet*> idToSheetMap;
};

#endif
