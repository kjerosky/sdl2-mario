#ifndef GAME_OBJECTS_MANAGER_H
#define GAME_OBJECTS_MANAGER_H

#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "Level.h"

class GameObjectsManager {

public:

    GameObjectsManager();
    ~GameObjectsManager();

    void add(GameObject *object);
    void destroy(GameObject *object);
    std::vector<GameObject*>* getObjectList();

    void updateAll(SDL_Point *cameraPosition);
    void processPendingAdditions();
    void drawObjects(SDL_Renderer *renderer, SDL_Point *cameraPosition);
    void drawTopmostObjects(SDL_Renderer *renderer, SDL_Point *cameraPosition);
    void destroyNonPlayerObjectsOutsideOfLevel(Level* level);
    void cleanupDestroyedObjects();

private:

    std::vector<GameObject*> objects;
    std::vector<GameObject*> objectsToAdd;
    std::vector<GameObject*> objectsToDestroy;
};

#endif
