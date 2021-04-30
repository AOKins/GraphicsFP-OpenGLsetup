#include "../headers/ship.h"

ship::ship() {
    this->mainComponent;
    this->mainNacelleRight;
    this->mainNacelleLeft;
    this->frontNacelleLeft;
    this->frontNacelleRight;
    this->status = OFF;
}

ship::ship(shader * objShader) {
    this->objects.clear();
    this->objects.push_back(new object("./resources/main_shipPart.obj","./resources/test.bmp", objShader));
    this->objects.push_back(new object("./resources/nacelle_shipPart.obj","./resources/test.bmp", objShader));
    this->objects.push_back(new object("./resources/nacelle_shipPart.obj","./resources/test.bmp", objShader));
    this->objects.push_back(new object("./resources/frontNacelle_shipPart.obj","./resources/test.bmp", objShader));
    this->objects.push_back(new object("./resources/frontNacelle_shipPart.obj","./resources/test.bmp", objShader));
    

    this->mainComponent = objects[0];
    this->mainComponent->name = "mainComponent";
    this->mainNacelleLeft = objects[1];
    this->mainNacelleLeft->name = "left Nacelle";
    this->mainNacelleRight = objects[2];
    this->mainNacelleRight->name = "right Nacelle";
    this->frontNacelleLeft = objects[3];
    this->frontNacelleLeft->name = "front left Nacelle";
    this->frontNacelleRight = objects[4];
    this->frontNacelleRight->name = "front right Nacelle";

    this->mainNacelleRight->setParent(this->mainComponent, glm::vec3(-0.7,0,0.40));
    this->mainNacelleLeft->setParent(this->mainComponent, glm::vec3(-0.7,0,-0.40));
    this->frontNacelleRight->setParent(this->mainNacelleRight, glm::vec3(0.636404,1.91865,0));
    this->frontNacelleLeft->setParent(this->mainNacelleLeft, glm::vec3(0.636404,1.91865,0));

    this->status = OFF;
    if (this->status == ON) {
        this->mainNacelleRight->setBank(0.8);
        this->mainNacelleLeft->setBank(-0.8);
    }
    else {
        this->mainNacelleRight->setBank(M_PI/2);
        this->mainNacelleLeft->setBank(-M_PI/2);
    }
}

void ship::setPos(glm::vec3 new_pos) {
    this->mainComponent->setPosition(new_pos);
}

void ship::setHeading(float new_heading) {
    this->mainComponent->setHeading(new_heading);
}

void ship::renderShip(shader * objShader, double ctime, double ltime) {
    this->frontNacelleLeft->setBank(15*ctime);
    this->frontNacelleRight->setBank(-15*ctime);

    this->setPos(glm::vec3(sin(ctime), 0,0));

    updateNacelleOri(ctime - ltime);

    for (int i = 0; i < objects.size(); i++) {
        objects[i]->renderObject(objShader);
    }
}

void ship::changeStatus() {
    switch(this->status) {
        case(TRANSITION_OFF):
            this->status = TRANSITION_ON;
            break;
        case(TRANSITION_ON):
            this->status = TRANSITION_OFF;
            break;
        case(OFF):
            this->status = TRANSITION_ON;
            break;
        case(ON):
            this->status = TRANSITION_OFF;
            break;
    }
}

void ship::updateNacelleOri(double deltaTime) {
    float curr_Bank = this->mainNacelleRight->getBank();
    if (this->status == TRANSITION_OFF) {
        curr_Bank += deltaTime;
        if (curr_Bank > M_PI/2) {
            curr_Bank = M_PI/2;
            this->status = OFF;
        }
    }
    else if (this->status == TRANSITION_ON) {
        curr_Bank -= deltaTime;
        if (curr_Bank < 0.8) {
            curr_Bank = 0.8;
            this->status = ON;
        }
    }
    this->mainNacelleLeft->setBank(-curr_Bank);
    this->mainNacelleRight->setBank(curr_Bank);
}

ship::~ship() {
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}