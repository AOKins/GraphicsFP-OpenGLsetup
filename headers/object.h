#ifndef _OBJFILE_H_
#define _OBJFILE_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/gl.h>


// Structure to hold properties of a rendered object
// Contents based on Scott's example of object loading
class object {
    float bank, heading, pitch; // Orientation of the object in radians
    glm::vec3 position; // Position in 3D space
    float scale;

    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 toWorld;
    
    void updateMatrices();

public:
    // Publicly accessible vertices, terxture vertices, and normals
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> normals;

    // Constuctor that takes in string to where the .obj file is located
    // Defaults the orientation to 0,0,0
    object(std::string filePath);
    // Method used in constructor to properly load contents of .obj file
    void load_from_file(std::string filePath);

    // Getters //
    float getBank();
    float getHeading();
    float getPitch();
    glm::vec3 getPosition();
    float getScale();
    glm::mat4 getTranslation();
    glm::mat4 getRotation();
    glm::mat4 getToWorld();

    // Setters //
    void setBank(float new_bank);
    void setHeading(float new_heading);
    void setPitch(float new_pitch);
    void setPosition(glm::vec3 new_pos);
    void setScale(float new_scale);

};

#endif