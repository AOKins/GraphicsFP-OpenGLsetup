#include "../headers/camera.h"
#include "transformDerive.cpp"
#include <iostream>

camera::camera() {
    this->position = glm::vec3(0.0f, 0.0f,1.0f);
    this->direction = glm::vec3(0.0f, 0.0f,-1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->fov = M_PI/2; // Default camera fov is 90

    // Defaulting the camera to 0 orientation
    this->bank = 0;
    this->pitch = 0;
    this->heading = -M_PI/2.0f;

    // Setting derived values by calling their update methods
    this->updateView();
}

// Setter for position
void camera::setPos(glm::vec4 new_pos) {
    this->position = new_pos;
    // With position changed, need to update the view
    this->updateView();
}

// Method for moving the camera forward (positive heading orientation direction)
void camera::moveFoward(float offset) {
    glm::vec3 forward_dir = glm::vec4(1.0f,0.0f,0.0f,1.0f) * getRotationY(this->heading);
    this->position = this->position + offset * forward_dir;
    updateView();
}

// Method for moving the camera left (positive heading orientation direction)
void camera::moveLeft(float offset) {
    glm::vec3 right_dir = glm::vec4(0.0f,0.0f,1.0f,1.0f) * getRotationY(this->heading);
    this->position = this->position - offset * right_dir;
    updateView();
}

// Method for moving the camera up (positive +y direction)
void camera::moveUp(float offset) {
    this->position = this->position + offset * this->up;
    updateView();
}

// Method for turning camera right (or left if negative argument)
void camera::turnRight(float degrees) {
    this->heading = this->heading + degrees;
    // Keeping heading within 2PI and -2PI
    if (this->heading > 2*M_PI) {
        this->heading -= 2*M_PI;
    }
    else if (this->heading < -2*M_PI) {
        this->heading += 2*M_PI;
    }
    updateView();
}

// Method for turning camera up (or down if negative argument)
void camera::turnUp(float degrees) {
    this->pitch = this->pitch - degrees;
    // Keeping heading to be less than PI/2 and PI/2 to prevent chance of gimple locking issues
    if (this->pitch > M_PI/2.1f) {
        this->pitch = M_PI/2.1f;
    }
    else if (this->pitch < -M_PI/2.1f) {
        this->pitch = -M_PI/2.1f;
    }
    updateView();
}

// Private method, should be used whenever changing position or orientation to update the view matrix for rendering
void camera::updateView() {
    this->up = this->abs_up;// * getRotationX(this->pitch);
    // Rotation is a bit different because the main axis is along Z as opposed to X
    this->direction = this->abs_foward * getRotationZ(this->pitch) * getRotationY(this->heading);
    this->view = glm::lookAt(this->position, this->direction + this->position, this->up);
}

// Simple getters for view matrix
glm::mat4 camera::getView() {
    return this->view;
}

// Simple setter for fov
void camera::setFOV(float new_fov) {
    this->fov = new_fov;
}

// Simple getter for fov
float camera::getFOV() {
    return this->fov;
}
