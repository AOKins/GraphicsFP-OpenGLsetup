#ifndef _OBJFILE_H_
#define _OBJFILE_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/gl.h>

#define GLEW_STATIC
#include <GL/glew.h>

// Structure to hold properties of a rendered object
// Contents based on Scott's example of object loading
class object {
    float bank, heading, pitch; // Orientation of the object in radians
    glm::vec3 position; // Position in 3D space
    float scale; // Scaler for the object

    bool textured;

    GLuint vertexArray_ID;
    GLuint verticesBuff_ID;
    GLuint uvBuff_ID;
    GLuint texture_ID;
    GLuint elementBuff_ID;

    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 toSpace;
    
    void updateMatrices();

public:
    // Publicly accessible vertices, terxture vertices, and normals
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> normals;

    // Constuctor that takes in string to where the .obj file is located and optional texture image file
    // Defaults the orientation to 0,0,0 as well as position
    object(std::string filePath, std::string textPath = "");
    ~object();
    // Method used in constructor to properly load contents of .obj file
    void load_from_file(std::string filePath);
    void load_texture(std::string textPath);
    
    // Getters //
    float getBank();
    float getHeading();
    float getPitch();
    glm::vec3 getPosition();
    float getScale();
    glm::mat4 getTranslation();
    glm::mat4 getRotation();
    glm::mat4 getToSpace();
    GLuint getVertexArrayID();
    GLuint getVertexBufferID();
    GLuint getUvBufferID();
    GLuint getElementBufferID();
    GLuint getTextureID();
    bool isTextured();

    // Setters //
    void setBank(float new_bank);
    void setHeading(float new_heading);
    void setPitch(float new_pitch);
    void setPosition(glm::vec3 new_pos);
    void setScale(float new_scale);

    void renderObject(int vertexID, int uvID);
};

#endif