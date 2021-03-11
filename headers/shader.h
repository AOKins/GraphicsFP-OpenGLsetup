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

    shader();
    ~shader();

    const char* load_from_file(const char * filePath);

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
};

#endif