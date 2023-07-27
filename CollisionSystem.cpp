#include "CollisionSystem.h"

#include "GameObject.h"

CollisionSystem* CollisionSystem::instance = NULL;

CollisionSystem *CollisionSystem::getInstance() {
    if (instance == NULL) {
        instance = new CollisionSystem();
    }
    return instance;
}

CollisionSystem::CollisionSystem() {
    // nothing to do for now
}

void CollisionSystem::initialize(std::vector<GameObject *> *gameObjectList) {
    this->gameObjectList = gameObjectList;
}

std::vector<CollisionResponse>* CollisionSystem::testObjectAgainstAllOthers(GameObject *source) {
    collisionResponses.clear();

    SDL_Rect sourceObjectWorldHitBox = *(source->getHitBox());
    sourceObjectWorldHitBox.x += source->getPosition()->x;
    sourceObjectWorldHitBox.y += source->getPosition()->y;

    for (std::vector<GameObject*>::iterator otherIterator = gameObjectList->begin(); otherIterator != gameObjectList->end(); otherIterator++) {
        GameObject* other = *otherIterator;
        if (other == source || !(other)->isEnabled() || !(other)->isCollidable()) {
            continue;
        }

        SDL_Rect otherWorldHitBox = *(other->getHitBox());
        otherWorldHitBox.x += other->getPosition()->x;
        otherWorldHitBox.y += other->getPosition()->y;
        if (SDL_HasIntersection(&sourceObjectWorldHitBox, &otherWorldHitBox)) {
            collisionResponses.push_back(other->receiveCollision(source));
        }
    }

    return &collisionResponses;
}
