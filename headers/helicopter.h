#ifndef _HELICOPTER_H_
#define _HELICOPTER_H_

#include "GLM/glm.hpp"
#include "object.h"


class helicopter {
private:
    glm::vec3 velocity;
    float mass;
    float throttle;

    object * body, *mainPropellor, *tailPropellor;
public:
    helicopter(object * set_Body, object * set_Prop, object * set_tailProp, float set_mass);
    
    void updateAnimation();
    void increaseThrottle(float inc);
    glm::vec3 getLift();
    glm::vec3 getVelocity();
    float getMass();
    
    void setVelocity(glm::vec3 new_vel);

    void setPosition(glm::vec3 new_pos);
    glm::vec3 getPos();

    void pitchForward(float change);
    void turnLeft(float change);
    
};


#endif
