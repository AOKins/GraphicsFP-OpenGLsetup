#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>

struct shader {
    // ID values
    GLuint shaderID;
    bool cancel;
    // Simple default destructor
    ~shader();
    // Constructor with given file names for the shaders, appends them to "./shaders/" filepath
    shader(const char * vertexName, const char * fragmentName);

    // Helpful method to read form a file, used to load shader source content
    const char* load_from_file(const char * filePath);
    
    // Simple setters inspired by learnopengl.com tutorial, helps in setting uniform values in the shader
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
    void setVec3(const std::string &name, glm::vec3 vector) const;

    // Getter for attribute location of given name
    GLuint getLocation(const std::string &name);
};

#endif