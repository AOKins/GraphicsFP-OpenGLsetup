#include "../headers/aabb3.h"

#include "./functions/transformDerive.cpp"

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

glm::vec3 aabb3::getCenter() {
    return (min + max) / 2.0f;
}

glm::vec3 aabb3::rotatedMin(float heading, float pitch, float bank) {
    glm::vec4 rot_min, rot_max;
    glm::vec3 new_min;
    glm::mat4 rotateMatrix = myRotationMethods::getRotationMatrix(pitch, heading,bank);

    rot_min = glm::vec4(this->min,1) * rotateMatrix;
    rot_max = glm::vec4(this->max,1) * rotateMatrix;

    new_min.x = std::min(rot_min.x, rot_max.x);
    new_min.y = std::min(rot_min.y, rot_max.y);
    new_min.z = std::min(rot_min.z, rot_max.z);
    return new_min;
}

glm::vec3 aabb3::rotatedMax(float heading, float pitch, float bank) {
    glm::vec4 rot_min, rot_max;
    glm::vec3 new_max;
    glm::mat4 rotateMatrix = myRotationMethods::getRotationMatrix(pitch, heading, bank);

    rot_min = glm::vec4(this->min,1) * rotateMatrix;
    rot_max = glm::vec4(this->max,1) * rotateMatrix;

    new_max.x = std::max(rot_min.x, rot_max.x);
    new_max.y = std::max(rot_min.y, rot_max.y);
    new_max.z = std::max(rot_min.z, rot_max.z);
    return new_max;
}
