#include "SpriteSheetRepository.h"

SpriteSheetRepository* SpriteSheetRepository::instance = NULL;

SpriteSheetRepository *SpriteSheetRepository::getInstance() {
    if (instance == NULL) {
        instance = new SpriteSheetRepository();
    }
    return instance;
}

SpriteSheetRepository::SpriteSheetRepository() {
    // nothing to do for now
}

bool SpriteSheetRepository::initialize(SDL_Renderer* renderer) {
    SpriteSheet* testLevelTilesSheet = new SpriteSheet("assets/test-level-tiles.png", 16, 16, renderer);
    SpriteSheet* smallMarioSheet = new SpriteSheet("assets/small-mario.png", 16, 16, renderer);
    SpriteSheet* superMarioSheet = new SpriteSheet("assets/super-mario.png", 16, 32, renderer);
    SpriteSheet* fireMarioSheet = new SpriteSheet("assets/fire-mario.png", 16, 32, renderer);
    SpriteSheet* fireballSheet = new SpriteSheet("assets/fireball.png", 16, 16, renderer);
    SpriteSheet* goombaSheet = new SpriteSheet("assets/goomba.png", 16, 16, renderer);

    bool successfullyInitialized =
        testLevelTilesSheet->wasLoadedSuccessfully() &&
        smallMarioSheet->wasLoadedSuccessfully() &&
        superMarioSheet->wasLoadedSuccessfully() &&
        fireMarioSheet->wasLoadedSuccessfully() &&
        fireballSheet->wasLoadedSuccessfully() &&
        goombaSheet->wasLoadedSuccessfully()
    ;
    if (!successfullyInitialized) {
        return false;
    }

    idToSheetMap[TEST_LEVEL_TILES_SHEET] = testLevelTilesSheet;
    idToSheetMap[SMALL_MARIO_SHEET] = smallMarioSheet;
    idToSheetMap[SUPER_MARIO_SHEET] = superMarioSheet;
    idToSheetMap[FIRE_MARIO_SHEET] = fireMarioSheet;
    idToSheetMap[FIREBALL_SHEET] = fireballSheet;
    idToSheetMap[GOOMBA_SHEET] = goombaSheet;

    return true;
}

SpriteSheet *SpriteSheetRepository::getSpriteSheetById(SpriteSheetId id) {
    return idToSheetMap[id];
}

void SpriteSheetRepository::cleanup() {
    for (std::map<SpriteSheetId, SpriteSheet*>::iterator entry = idToSheetMap.begin(); entry != idToSheetMap.end(); entry++) {
        delete entry->second;
    }
}
