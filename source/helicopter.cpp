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

    // Setting the bounding boxes
    this->bodyAABB3.push_back(aabb3());
    this->bodyAABB3[0].min = glm::vec3(-1.37,-1.41,-1.18);
    this->bodyAABB3[0].max = glm::vec3( 1.47,-1.25, 1.18);

    this->bodyAABB3.push_back(aabb3());
    this->bodyAABB3[1].min = glm::vec3(-3,-1,-1);
    this->bodyAABB3[1].max = glm::vec3(1,1,1);

    this->bodyAABB3.push_back(aabb3());
    this->bodyAABB3[2].min = glm::vec3(-5.2,0.32,0.2);
    this->bodyAABB3[2].max = glm::vec3(-4.2,1.21,-0.2);

    this->propellorAABB3.min = glm::vec3(-4,1.18,-4);
    this->propellorAABB3.max = glm::vec3(4,1.22,4);
}

void helicopter::setupHelicopter(float seed) {
    srand(seed);
    glm::vec3 iPos;
    iPos.x = float(rand() % 200 - 100);
    iPos.z = float(rand() % 200 - 100);
    iPos.y = float(rand() % 200 - 100)/100.0f+10.0f;

    this->setPosition(iPos);
    if (seed == 1) {
        this->setVelocity(glm::vec3(0));
    }
    else {
        glm::vec3 iVel;
        iVel.x = float(rand() % 200 - 100);
        iVel.y = float(rand() % 200 - 100);
        iVel.z = float(rand() % 200 - 100);
        this->setVelocity(iVel);
    }
    this->setThrottle(39.24f);

    this->body->setPitch(0);
    this->body->setBank(0);
}

void helicopter::updateAnimation() {
    this->mainPropellor->setHeading(this->mainPropellor->getHeading()+this->throttle);
    this->tailPropellor->setPitch(this->tailPropellor->getPitch()+2.0f*this->throttle);
}

void helicopter::increaseThrottle(float inc) {
    this->throttle += inc;
}

void helicopter::setThrottle(float set) {
    this->throttle = set;
}

glm::vec3 helicopter::getLift() {
    glm::vec4 up = glm::vec4(0,1,0,1);
    glm::vec3 up_ori = up * myRotationMethods::getRotationZ(-this->body->getPitch()) * myRotationMethods::getRotationY(-this->body->getHeading()) * myRotationMethods::getRotationX(-this->body->getBank());
    return 1000 * this->throttle * glm::normalize(up_ori);
}

void helicopter::setVelocity(glm::vec3 new_vel) {
    this->velocity = new_vel;
}

glm::vec3 helicopter::getVelocity() {
    return this->velocity;
}

float helicopter::getSpeed() {
    return sqrt(this->velocity.x*this->velocity.x + this->velocity.y*this->velocity.y + this->velocity.z*this->velocity.z);
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
void helicopter::bankLeft(float change) {
    float current = body->getBank();
    body->setBank(current + change);
}

int helicopter::collidedGround() {
    glm::vec3 worldPos = this->body->getPosition();
    for (int i=0; i < this->bodyAABB3.size(); i++) {
        if ((worldPos + this->bodyAABB3[i].rotatedMin(this->body->getHeading(),this->body->getPitch(),this->body->getBank())).y <= 0 ||
            (worldPos + this->bodyAABB3[i].rotatedMax(this->body->getHeading(),this->body->getPitch(),this->body->getBank())).y <= 0 
        ) {
            return i + 1;
        }
    }
    
    if ((worldPos + this->propellorAABB3.rotatedMin(this->body->getHeading(),this->body->getPitch(),this->body->getBank())).y <= 0 ||
        (worldPos + this->propellorAABB3.rotatedMax(this->body->getHeading(),this->body->getPitch(),this->body->getBank())).y <= 0 
    ) {
        return this->bodyAABB3.size() + 1;
    }
    else {
        return 0;
    }

}