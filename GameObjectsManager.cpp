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
