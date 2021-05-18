#include "../../headers/helicopter.h"
#include <GLM/glm.hpp>

namespace PSmethods
{

void updateHelicopter(helicopter * heli, float deltaTime) {
    glm::vec3 currPos = heli->getPos();
    glm::vec3 currVel = heli->getVelocity();
    currPos = currPos + currVel * deltaTime/100.0f;
    currVel = currVel + heli->getLift() * deltaTime/100.0f - glm::vec3(0,9.81f,0) * heli->getMass() * deltaTime/100.0f;
    heli->setPosition(currPos);
    heli->setVelocity(currVel);
}

}
