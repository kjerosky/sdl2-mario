#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include <vector>

// NOTE: There's a circular dependency between GameObject and CollisionSystem,
//       so we can't include GameObject.h here. Instead, we'll forward declare
//       the GameObject class here for now so it compiles. ¯\_(ツ)_/¯
class GameObject;

enum CollisionResponseType {
    NO_PROBLEM,
    REVERSE_COURSE,
    GET_STOMPED,
    REACT_TO_STOMP,
    TAKE_DAMAGE,
    POWER_UP,
    GET_CONSUMED,
};

typedef struct CollisionResponse {
    CollisionResponseType type;
    GameObject* source;
} CollisionResponse;

class CollisionSystem {

public:

    static CollisionSystem* getInstance();
    void initialize(std::vector<GameObject*>* gameObjectList);

    std::vector<CollisionResponse>* testObjectAgainstAllOthers(GameObject* source);

private:

    CollisionSystem();
    static CollisionSystem* instance;

    std::vector<GameObject*>* gameObjectList;
    std::vector<CollisionResponse> collisionResponses;
};

#endif
