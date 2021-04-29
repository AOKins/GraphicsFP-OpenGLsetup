#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <string>
#include <vector>
#include "shader.h"

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
    // IDs for the vertex arrays
    GLuint vertexArray_ID, normalArray_ID;
    // IDs for the buffers themselves
    GLuint verticiesBuff_ID, normalBuff_ID, uvBuff_ID;
    GLuint texture_ID;
    GLuint elementBuff_ID;

    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 toSpace;
    
    void updateMatrices();

    // Property to connecting this object to a parent object for more complex objects
    object * parentObj;
    glm::vec3 hierTranslate;

public:
    // Publicly accessible vertices, terxture vertices, and normals
    std::vector<glm::vec4> verticies;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec4> normals;

    std::vector<glm::vec4> facePoints;
    std::vector<glm::vec4> faceNormals;

    object();
    // Constuctor that takes in string to where the .obj file is located and optional texture image file
    // Defaults the orientation to 0,0,0 as well as position
    object(std::string objPath, std::string textPath, shader * objShader);
    ~object();
    // Method used in constructor to properly load contents of .obj file
    void load_from_file(std::string filePath);
    void load_texture(std::string textPath);
    
    // Method used in constructor to derive contents for facePoints and faceNormals
    void deriveFacePoints();
    // Method for getting the distance from a point to a given face
    // Input: c_pos - 3D point to check
    //        face_id - the given face to check
    // Output: distance from the face
    float getDistanceFromFace(glm::vec3 c_pos, int face_id);

    // Getters //
    glm::vec3 getPosition();
    
    float getBank();
    float getHeading();
    float getPitch();
    float getScale();
    
    glm::mat4 getObjectTranslation();
    glm::mat4 getHierarchyTranslation();
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
    // Setter for parent object with option argument for translation
    void setParent(object * new_parent, glm::vec3 setHierTranslate = glm::vec3(0,0,0));

    // Method for handling the rendering of this object with a given shader
    void renderObject(shader * objShader);
};

#endif