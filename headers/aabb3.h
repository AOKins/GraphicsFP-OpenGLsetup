#ifndef _AABB3_H_
#define _AABB3_H_

#include "GLM/glm.hpp"

// Struct to hold info on bounding box in 3 dimensions
struct aabb3 {
    glm::vec3 min;
    glm::vec3 max;

    // Constructor uses object to derive min and max points
    aabb3() {
        this->max = glm::vec3(0);
        this->min = glm::vec3(0);
    }

    // Method for getting center point
    glm::vec3 getCenter();

    glm::vec3 rotatedMin(float heading, float pitch, float bank);
    glm::vec3 rotatedMax(float heading, float pitch, float bank);
};

bool intersect(const aabb3 &a, const aabb3 &b);

#endif
