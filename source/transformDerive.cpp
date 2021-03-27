#ifndef _TRANSFORMS_CPP_
#define _TRANSFORMS_CPP_

#include <GLM/glm.hpp>


glm::mat4 getRotationMatrix(float bank, float heading, float pitch);
glm::mat4 getRotationY(float angle);
glm::mat4 getRotationX(float angle);
glm::mat4 getRotationZ(float angle);


glm::mat4 getRotationMatrix(float bank, float heading, float pitch) {
    return getRotationZ(bank) * getRotationX(pitch) * getRotationY(heading);
}

glm::mat4 getRotationX(float angle) {
    return glm::mat4x4( // Rotation about the x axis
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  glm::cos(angle), glm::sin(angle), 0.0f,
        0.0f, -glm::sin(angle), glm::cos(angle), 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f);
}

glm::mat4 getRotationY(float angle) {
    return glm::mat4x4( // Rotation about the y axis
        glm::cos(angle), 0.0f, -glm::sin(angle), 0.0f,
        0.0f,                 1.0f,                  0.0f, 0.0f,
        glm::sin(angle), 0.0f,  glm::cos(angle), 0.0f,
        0.0f,                 0.0f,                  0.0f, 1.0f);
}

glm::mat4 getRotationZ(float angle) {
    return glm::mat4x4( // Rotation about the z axis
         glm::cos(angle), glm::sin(angle), 0.0f, 0.0f,
        -glm::sin(angle), glm::cos(angle), 0.0f, 0.0f,
          0.0f,                 0.0f, 1.0f, 0.0f,
          0.0f,                 0.0f, 0.0f, 1.0f);
}

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
