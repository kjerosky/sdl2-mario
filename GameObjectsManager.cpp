#include "GameObjectsManager.h"

GameObjectsManager::GameObjectsManager() {
    // do nothing for now
}

GameObjectsManager::~GameObjectsManager() {
    for (std::vector<GameObject*>::iterator object = objects.begin(); object != objects.end(); object++) {
        delete *object;
    }
}

void GameObjectsManager::add(GameObject *object) {
    objects.push_back(object);
}

void GameObjectsManager::destroy(GameObject *object) {
    objectsToDestroy.push_back(object);
}

std::vector<GameObject*>* GameObjectsManager::getObjectList() {
    return &objects;
}

void GameObjectsManager::updateAll(SDL_Point *cameraPosition) {
    for (std::vector<GameObject*>::iterator object = objects.begin(); object != objects.end(); object++) {
        (*object)->update(cameraPosition);
    }
}

void GameObjectsManager::drawAll(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    for (std::vector<GameObject*>::iterator object = objects.begin(); object != objects.end(); object++) {
        (*object)->draw(renderer, cameraPosition);
    }
}

void GameObjectsManager::cleanupDestroyedObjects() {
    for (std::vector<GameObject*>::iterator objectToDestroy = objectsToDestroy.begin(); objectToDestroy != objectsToDestroy.end(); objectToDestroy++) {
        std::vector<GameObject*>::iterator targetObject = std::find(objects.begin(), objects.end(), *objectToDestroy);
        if (targetObject != objectsToDestroy.end()) {
            delete *targetObject;
            objects.erase(targetObject);
        }
    }

    if (!objectsToDestroy.empty()) {
        objectsToDestroy.clear();
    }
}
