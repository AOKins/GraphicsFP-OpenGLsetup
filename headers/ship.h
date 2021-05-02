#ifndef _SHIP_H_
#define _SHIP_H_

#include "object.h"
#include "shader.h"

enum ENGINE_POS {
    OFF,
    TRANSITION_OFF,
    TRANSITION_ON,
    ON
};

struct ship {
private:
    object * mainComponent;
    object * mainNacelleLeft, * mainNacelleRight;
    object * frontNacelleLeft, * frontNacelleRight;
    std::vector<object*> objects;
    ENGINE_POS status;
    float instability;

    void updateNacelleOri(double cTime, double lTime);

public:
    ship();
    ship(shader * objShader);
    ~ship();

    // Setting
    void setPos(glm::vec3 new_pos);
    void setHeading(float new_heading);
    void changeStatus();
    void setInstability(float set_inst);
    // Getter for position
    glm::vec3 getPos();

    void renderShip(shader * objShader, double ctime, double ltime);
};

#endif
