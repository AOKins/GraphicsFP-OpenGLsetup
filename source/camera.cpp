#include "../headers/camera.h"
#include <iostream>

camera::camera() {
    this->position = glm::vec3(0.0f, 0.0f,1.0f);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->fov = 90.0f; // Default camera fov is 90

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
    this->view = glm::lookAt(this->position, this->position + this->front, this->up);
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
