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
};

// Method for determining if there is an intersection between two bounding boxes
bool intersect(const aabb3 &a, const aabb3 &b) {
    // Follows example code in Listing A.5 of 3D Math Primer for Graphics and Game Development by Fletcher Dunn & Ian Parberry

    if (a.min.x >= b.max.x) return false;
    if (a.max.x <= b.min.x) return false;
    if (a.min.y >= b.max.y) return false;
    if (a.max.y <= b.min.y) return false;
    if (a.min.z >= b.max.z) return false;
    if (a.max.z <= b.min.z) return false;
    
    return true;
}

#endif
