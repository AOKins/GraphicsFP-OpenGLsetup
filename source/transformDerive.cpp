#ifndef _TRANSFORMS_CPP_
#define _TRANSFORMS_CPP_
// Methods for generating rotation matrices
#include <GLM/glm.hpp>

// Generate rotation matrix using euler angles, relies on getRotationY, getRotationX, and getRotationZ methods
// Input: bank, heading, pitch - euler angles for rotation
// Output: A 4x4 matrix of rotation is returned
glm::mat4 getRotationMatrix(float bank, float heading, float pitch);

// Generate rotation matrix that is for rotation around Y axis given angle
glm::mat4 getRotationY(float angle);
// Generate rotation matrix that is for rotation around X axis given angle
glm::mat4 getRotationX(float angle);
// Generate rotation matrix that is for rotation around Z axis given angle
glm::mat4 getRotationZ(float angle);

// Generate rotation matrix that is for rotation around a given axis
glm::mat3 getRotataionN(float angle, glm::vec3 axis);

// Generate rotation matrix using euler angles, relies on getRotationY, getRotationX, and getRotationZ methods
// Input: bank, heading, pitch - euler angles for rotation
// Output: A 4x4 matrix of rotation is returned
glm::mat4 getRotationMatrix(float bank, float heading, float pitch) {
    return getRotationY(heading) * getRotationZ(pitch) * getRotationX(bank);
}

// Generate rotation matrix that is for rotation around X axis given angle
glm::mat4 getRotationX(float angle) {
    return glm::mat4x4( // Rotation about the x axis
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  glm::cos(angle), glm::sin(angle), 0.0f,
        0.0f, -glm::sin(angle), glm::cos(angle), 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f);
}

// Generate rotation matrix that is for rotation around Y axis given angle
glm::mat4 getRotationY(float angle) {
    return glm::mat4x4( // Rotation about the y axis
        glm::cos(angle), 0.0f, -glm::sin(angle), 0.0f,
        0.0f,            1.0f,             0.0f, 0.0f,
        glm::sin(angle), 0.0f,  glm::cos(angle), 0.0f,
        0.0f,            0.0f,             0.0f, 1.0f);
}

// Generate rotation matrix that is for rotation around Z axis given angle
glm::mat4 getRotationZ(float angle) {
    return glm::mat4x4( // Rotation about the z axis
         glm::cos(angle), glm::sin(angle), 0.0f, 0.0f,
        -glm::sin(angle), glm::cos(angle), 0.0f, 0.0f,
          0.0f,                 0.0f, 1.0f, 0.0f,
          0.0f,                 0.0f, 0.0f, 1.0f);
}

// Generate rotation matrix that is for rotation around a given axis
glm::mat3 getRotataionN(float angle, glm::vec3 axis) {
    return glm::mat3(
        // Row 1
        axis.x*axis.x*(1 - glm::cos(angle)) + glm::cos(angle),
        axis.x*axis.y*(1 - glm::cos(angle)) + axis.z*glm::sin(angle),
        axis.x*axis.z*(1 - glm::cos(angle)) - axis.y*glm::sin(angle),
        // Row 2
        axis.x*axis.y*(1 - glm::cos(angle)) - axis.z*glm::sin(angle),
        axis.y*axis.y*(1 - glm::cos(angle)) + glm::cos(angle),
        axis.y*axis.z*(1 - glm::cos(angle)) - axis.x*glm::sin(angle),
        // Row 3
        axis.x*axis.z*(1 - glm::cos(angle)) + axis.y*glm::sin(angle),
        axis.y*axis.z*(1 - glm::cos(angle)) + axis.x*glm::sin(angle),
        axis.z*axis.z*(1 - glm::cos(angle)) + glm::cos(angle)
    );
}

#endif
