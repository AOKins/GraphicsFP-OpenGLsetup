#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

enum AXIS {
    X,
    Y,
    Z
};

// Class object to encapsulates general camera behavior
class camera {
private:
    // Camera properties for position and orientation
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 direction;
    glm::mat4 view;

    // Private methods used by setters to manage view transformation matrix and direction vector data
    void updateView();
    void updateDirection();

    // Direction that is right of the camera's perspective
    glm::vec3 right;
    // Direction that is up to the camera's perspective
    glm::vec3 up;
    // Direction that is taken to be absolute up and forward
    const glm::vec3 abs_up = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::vec4 abs_front = glm::vec4(0.0f, 0.0f,-1.0f, 1.0f);

    float fov; // Field of view of the camera

    float bank, heading, pitch; // Orientation of the camera in radians

public:
    // A constructor for camera that simply takes in position
    camera();

    // Setters
    void setPos(glm::vec3 new_pos);
    void setFront(glm::vec3 new_orientation);
    void setUp(glm::vec3 new_up);
    void setFOV(float new_pers);

    // Updates the position of the camera by offset along given axis
    void updatePos(double offset, AXIS axis);

    // Getters
    glm::vec3 getPos();
    glm::vec3 getDirection();
    // View uses glm::lookat() with this camera's position and target
    glm::mat4 getView();
    glm::vec3 getRight();
    glm::vec3 getFront();
    void updateRight();
    glm::vec3 getUp();
    void updateUp();
    float getFOV();

    void turnLeftRight(float angle);
    void turnUpDown(float angle);
    void turnRotate(float angle);

};


#endif