#include "../headers/camera.h"
#include "transformDerive.cpp"

camera::camera() {
    this->position = glm::vec3(0.0f, 0.0f,1.0f);
    this->front = glm::vec3(0.0f, 0.0f, -0.1f);
    this->fov = 90.0f; // Default camera fov is 90

    this->bank = 0;
    this->pitch = 0;
    this->heading = 0;

    // Setting derived values by calling their update methods
    this->updateDirection();
    this->updateRight();
    this->updateUp();
    this->updateView();
}

void camera::setFront(glm::vec3 new_orientation) {
    this->front = new_orientation;
    this->updateDirection();
    this->updateRight();
    this->updateUp();
    this->updateView();
}

void camera::setPos(glm::vec3 new_pos) {
    this->position = new_pos;
    // With position changed, need to update the view
    this->updateView();
}

void camera::updatePos(double offset, AXIS axis) {
    switch(axis) {
        case(X):
            position.x += offset;
            break;
        case(Y):
            position.y += offset;
            break;
        case(Z):
            position.z += offset;
            break;
    }
    this->updateView();    // With position changed, need to update the view
}

glm::vec3 camera::getPos() {
    return this->position;
}

glm::vec3 camera::getFront() {
    return this->front;
}

void camera::updateDirection() {
    this->direction = glm::normalize(this->position - this->front);
}

void camera::updateView() {
    this->view = glm::lookAt(this->position, this->front + this->position, this->up);
}

void camera::updateRight() {
    this->right = glm::normalize(glm::cross(this->abs_up, this->direction));
}

void camera::updateUp() {
    this->up = glm::cross(this->direction, this->right);
}

glm::mat4 camera::getView() {
    return this->view;
}

void camera::setFOV(float new_fov) {
    this->fov = new_fov;
}

float camera::getFOV() {
    return this->fov;
}

void camera::turnLeftRight(float angle) {
    this->heading = this->heading + angle;
    if (this->heading >= 2*M_PI) {
        this->heading -= 2*M_PI;
    }
    else if (this->heading <= -2*M_PI) {
        this->heading += 2*M_PI;
    }
    setFront(abs_front * getRotataionN(this->pitch, this->right) * getRotataionN(this->heading, this->abs_up));
}

void camera::turnUpDown(float angle) {
    this->pitch = this->pitch + angle;
    if (this->pitch >= M_PI/4.0f) {
        this->pitch = M_PI/4.0f;
    }
    else if (this->pitch <= -M_PI/4.0f) {
        this->pitch = -M_PI/4.0f;
    }
    setFront(abs_front * getRotataionN(this->pitch, this->right) * getRotataionN(this->heading, this->abs_up));
}

