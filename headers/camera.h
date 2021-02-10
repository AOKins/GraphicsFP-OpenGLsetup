#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

// Class object to encapsulates general camera behavior
class Camera {
private:
    // Camera properties for position and orientation
    glm::vec3 position;
    glm::vec3 target;
    // Direction is the vector describing where the camera is pointing at
    glm::vec3 direction;
    glm::mat4 view;
    void updateView();
    void updateDirection();

    // Direction that is right of the camera's perspective
    glm::vec3 right;
    // Direction that is up to the camera's perspective
    glm::vec3 up;
    // Direction that is taken to be absolute up
    const glm::vec3 abs_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float perspective;
public:
    // A constructor for camera that simply takes in position
    Camera(glm::vec3 i_pos);

    // Setters
    void setPos(glm::vec3 new_pos);
    void setTarget(glm::vec3 new_target);
    void setUp(glm::vec3 new_up);
    void setPerspective(float new_pers);

    // Getters
    glm::vec3 getPos();
    glm::vec3 getTarget();
    glm::vec3 getDirection();
    // View uses glm::lookat() with this camera's position and target
    glm::mat4 getView();
    glm::vec3 getRight();
    void updateRight();
    glm::vec3 getUp();
    void updateUp();
    float getPerspective();
    
};

Camera::Camera(glm::vec3 i_pos) {
    this->position = i_pos;
    this->target = glm::vec3(0.0f,0.0f,0.0f);
    this->perspective = 45.0f; // Default camera fov is 45

    // Setting derived values by calling their update methods
    this->updateDirection();
    this->updateView();
    this->updateRight();
    this->updateUp();
}

void Camera::setTarget(glm::vec3 new_target) {
    this->target = new_target;
    this->updateDirection();
    this->updateView();
    this->updateRight();
    this->updateUp();
}

void Camera::setPos(glm::vec3 new_pos) {
    this->position = new_pos;
    // With position changed, need to update the direction and view properties
    this->updateDirection();
    this->updateView();
    this->updateRight();
    this->updateUp();
}

glm::vec3 Camera::getPos() {
    return this->position;
}

glm::vec3 Camera::getTarget() {
    return this->target;
}

void Camera::updateDirection() {
    this->direction = glm::normalize(this->position - this->target);
}

void Camera::updateView() {
    this->view = glm::lookAt(this->position, this->target, this->up);
}

void Camera::updateRight() {
    this->right = glm::normalize(glm::cross(this->abs_up, this->direction));
}

void Camera::updateUp() {
    this->up = glm::cross(this->direction, this->right);
}

glm::mat4 Camera::getView() {
    return this->view;
}

void Camera::setPerspective(float new_pers) {
    this->perspective = new_pers;
}

float Camera::getPerspective() {
    return this->perspective;
}

#endif
