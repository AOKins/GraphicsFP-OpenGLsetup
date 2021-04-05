#include "../headers/camera.h"
#include "./functions/transformDerive.cpp"
#include <math.h>

camera::camera() {
    this->position = glm::vec3(0.0f, 0.0f,1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->fov = M_PI/2; // Default camera fov is 90
    this->aspect = float(16)/float(9); // Default ratio to 16:9
    this->clip_far = 100.0f;
    this->clip_near = 0.1f;
    
    // Defaulting the camera to 0 orientation
    this->bank = 0;
    this->pitch = 0;
    this->heading = -M_PI/2.0f;

    // Setting derived values by calling their update methods
    this->updateView();
}

// Only sets new ratio if it is greater than 0
void camera::setAspect(float new_ratio) {
    if (new_ratio > 0) {
        this->aspect = new_ratio;
        this->updateView();
    }
}

// Only sets new clipping distance if it is greater than 0
void camera::setClipFar(float new_clipFar) {
    if (new_clipFar > 0) {
        this->clip_far = new_clipFar;
        this->updateView();
    }
}

// Only sets new clipping distance if it is greater than 0
void camera::setClipNear(float new_clipNear) {
    if (new_clipNear > 0) {
        this->clip_near = new_clipNear;
    }
}


// Setter for position
void camera::setPos(glm::vec4 new_pos) {
    this->position = new_pos;
    // With position changed, need to update the view
    this->updateView();
}

// Setter for heading
void camera::setHeading(float angle) {
    this->heading = angle;
    this->updateView();
}

// Setter for bank
void camera::setBank(float angle) {
    this->bank = angle;
    this->updateView();
}

// Setter for pitch
void camera::setPitch(float angle) {
    this->pitch = angle;
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
    glm::vec3 direction = this->abs_foward * getRotationZ(this->pitch) * getRotationY(this->heading);
    // Update view matrix (projection + translation)
    this->view = glm::lookAt(this->position, direction + this->position, this->up);

    // Setting projection to have projection contents of view matrix, but ommitting translation elements
    this->projection = glm::mat4(
        glm::vec4(view[0]),
        glm::vec4(view[1]),
        glm::vec4(view[2]),
        glm::vec4(0,0,0,1)
    );

    // Update perspective matrix
    this->perspective = glm::perspective(this->fov, this->aspect, this->clip_near, this->clip_far);
}

// Simple getters for view matrix
glm::mat4 camera::getView() {
    return this->view;
}

glm::mat4 camera::getPerspective() {
    return this->perspective;
}

// Simple setter for fov
void camera::setFOV(float new_fov) {
    this->fov = new_fov;
}

// Simple getter for fov
float camera::getFOV() {
    return this->fov;
}
