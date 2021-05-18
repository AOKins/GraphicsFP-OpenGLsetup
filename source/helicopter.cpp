#include "../headers/helicopter.h"
#include "./functions/transformDerive.cpp"

helicopter::helicopter(object * set_Body, object * set_Prop, object * set_tailProp, float set_mass) {
    this->body = set_Body;
    this->mainPropellor = set_Prop;
    this->tailPropellor = set_tailProp;
    this->mass     = set_mass;
    this->throttle = 0.0f;
    this->body->setPosition(glm::vec3(0,2.0f,1.0f));
    this->velocity = glm::vec3(0);
}

void helicopter::updateAnimation() {
    this->mainPropellor->setHeading(this->mainPropellor->getHeading()+this->throttle);
    this->tailPropellor->setPitch(this->tailPropellor->getPitch()+2.0f*this->throttle);
}

void helicopter::increaseThrottle(float inc) {
    this->throttle += inc;
}

glm::vec3 helicopter::getLift() {
    glm::vec4 up = glm::vec4(0,1,0,1);
    glm::vec3 up_ori = up * getRotationZ(-this->body->getPitch()) * getRotationY(-this->body->getHeading());
    return 1000 * this->throttle * glm::normalize(up_ori);
}

void helicopter::setVelocity(glm::vec3 new_vel) {
    this->velocity = new_vel;
}

glm::vec3 helicopter::getVelocity() {
    return this->velocity;
}

float helicopter::getMass() {
    return this->mass;
}

void helicopter::setPosition(glm::vec3 new_pos) {
    this->body->setPosition(new_pos);
}

glm::vec3 helicopter::getPos() {
    return this->body->getPosition();
}

void helicopter::pitchForward(float change) {
    float current = body->getPitch();
    body->setPitch(current - change);
}

void helicopter::turnLeft(float change) {
    float current = body->getHeading();
    body->setHeading(current + change);
}