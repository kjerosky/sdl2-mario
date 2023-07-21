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
    objectsToAdd.push_back(object);
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

void GameObjectsManager::processPendingAdditions() {
    for (std::vector<GameObject*>::iterator objectToAdd = objectsToAdd.begin(); objectToAdd != objectsToAdd.end(); objectToAdd++) {
        objects.push_back(*objectToAdd);
    }

    if (!objectsToAdd.empty()) {
        objectsToAdd.clear();
    }
}

void GameObjectsManager::drawObjects(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    for (std::vector<GameObject*>::iterator object = objects.begin(); object != objects.end(); object++) {
        GameObject* theObject = *object;
        if (!theObject->isDrawnOnTop()) {
            theObject->draw(renderer, cameraPosition);
        }
    }
}

void GameObjectsManager::drawTopmostObjects(SDL_Renderer *renderer, SDL_Point *cameraPosition) {
    for (std::vector<GameObject*>::iterator object = objects.begin(); object != objects.end(); object++) {
        GameObject* theObject = *object;
        if (theObject->isDrawnOnTop()) {
            theObject->draw(renderer, cameraPosition);
        }
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
