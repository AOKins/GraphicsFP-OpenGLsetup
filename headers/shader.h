#ifndef _SHADER_H_
#define _SHADER_H_

#define GLEW_STATIC
#include <GL/glew.h>

struct shader {
    // ID values
    GLuint shaderID;

    shader();

    const char* load_from_file(const char * filePath);
};

#endif