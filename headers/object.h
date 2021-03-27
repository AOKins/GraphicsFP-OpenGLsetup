#ifndef _OBJFILE_H_
#define _OBJFILE_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/gl.h>

// Structure to hold properties of a rendered object
// Contents based on Scott's example of object loading
struct object {
    float bank, heading, pitch; // Orientation of the object in radians
    float x,y,z; // Position in 3D space
    float scale;
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> normals;

public:
    // Constuctor that takes in string to where the .obj file is located
    // Defaults the orientation to 0,0,0
    object(std::string filePath);
    // Method used in constructor to properly load contents of .obj file
    void load_from_file(std::string filePath);
};

#endif