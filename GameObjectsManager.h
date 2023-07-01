#ifndef GAME_OBJECTS_MANAGER_H
#define GAME_OBJECTS_MANAGER_H

#include "GameObject.h"
#include "vector"
#include <SDL.h>

class GameObjectsManager {

public:

    GameObjectsManager();
    ~GameObjectsManager();

    void add(GameObject *object);

    void updateAll(SDL_Point *cameraPosition);
    void drawAll(SDL_Renderer *renderer, SDL_Point *cameraPosition);

private:

    std::vector<GameObject*> objects;
};

#endif
