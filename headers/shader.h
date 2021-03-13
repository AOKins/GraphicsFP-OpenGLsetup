#ifndef _SHADER_H_
#define _SHADER_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

struct shader {
    // ID values
    GLuint shaderID;
    bool cancel;
    // Simple default constructor and destructor
    shader();
    ~shader();

    // Helpful method to read form a file, used to load shader source content
    const char* load_from_file(const char * filePath);
    
    // Simple setters based on learnopengl.com tutorial, helps in setting uniform values in the shader
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
};

#endif