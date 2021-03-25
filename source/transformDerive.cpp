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