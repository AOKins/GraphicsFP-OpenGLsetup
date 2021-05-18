#ifndef _HELICOPTER_H_
#define _HELICOPTER_H_

#include "GLM/glm.hpp"
#include "object.h"
#include "aabb3.h"
#include <vector>

class helicopter {
private:
    glm::vec3 velocity;
    float mass;
    float throttle;

    object * body, *mainPropellor, *tailPropellor;
    std::vector<aabb3> bodyAABB3;
    aabb3 propellorAABB3;    
public:
    helicopter(object * set_Body, object * set_Prop, object * set_tailProp, float set_mass);
    
    void setupHelicopter(float seed);

    void updateAnimation();
    void increaseThrottle(float inc);
    void setThrottle(float set);
    glm::vec3 getLift();
    glm::vec3 getVelocity();
    float getSpeed();
    float getMass();
    
    void setVelocity(glm::vec3 new_vel);

    void setPosition(glm::vec3 new_pos);
    glm::vec3 getPos();

    void pitchForward(float change);
    void turnLeft(float change);
    void bankLeft(float change);

    // Returns 0 if not collided, nonzero number with 1 being landing gear, 2 being body, 3 being tail end, and 4 being propellors
    int collidedGround();
    
};


#endif
