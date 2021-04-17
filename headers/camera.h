#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

// Class object to encapsulates general camera behavior
class camera {
private:
    // Camera properties for position and orientation
    glm::vec3 position;
    glm::vec3 up;    // Direction that is up to the camera's perspective

    float bank, heading, pitch; // Orientation of the camera in radians
    float fov; // Field of view of the camera
    float aspect;
    float clip_far;
    float clip_near;

    glm::mat4 view; // Matrix for storing resulting projection and translation for rendering
    glm::mat4 projection; // Projection without translation (used in skybox case for example)
    glm::mat4 perspective; // Perspective/Projection for camera orientation

    // Private methods used by setters to update view transformation matrices when values change
    void updateView();

    // Direction that is taken to be absolute up and forward reference points
    const glm::vec4 abs_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    const glm::vec4 abs_foward = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
public:
    // A constructor for camera that simply takes in position
    camera();

    // Simple Setters
    void setPos(glm::vec4 new_pos);
    void setFOV(float new_pers);
    void setAspect(float new_ratio);
    void setClipNear(float new_clipNear);
    void setClipFar(float new_clipFar);
    void setHeading(float new_heading);
    void setBank(float new_bank);
    void setPitch(float new_pitch);

    // Simple Getters
    glm::vec4 getDirection();
    glm::vec4 getUp();
    float getAspect();
    float getClipNear();
    float getClipFar();

    float getFOV();

    // View uses glm::lookat() with this camera's position and target
    glm::mat4 getView();

    glm::mat4 getPerspective();

    glm::mat4 getProjection();

    glm::vec3 getPosition();

    // Camera control methods for changing view //
    // Note: camera currently works somewhat first person perspective, maintaining up in the +y direction

    // Method for turning camera right (or left if negative argument)
    void turnRight(float degrees);
    // Method for turning camera up (or down if negative argument)
    void turnUp(float degrees);
    // Method for moving the camera forward (positive heading orientation direction)
    void moveFoward(float offset);
    // Method for moving the camera left (positive heading orientation direction)
    void moveLeft(float offset);
    // Method for moving the camera up (positive +y direction)
    void moveUp(float offset);

};


#endif
