#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// Overall structure inspired/provided by https://learnopengl.com/Getting-started/Shaders

class Shader {
private:
    unsigned int ID;
public:
    Shader(const char* vertexFilePath, const char* fragmentFilePath);
    ~Shader();

    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    GLuint getID();
};

// Constructor, takes in paths to vertex shader and fragment shader
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
    std::string vertexCode, fragCode, line;
    std::ifstream vertexFile, fragFile;
    std::stringstream vertStream, fragStream;

    int success;
    char errorLog[512];

    vertexFile.open(vertexFilePath);
    vertStream << vertexFile.rdbuf();
    vertexCode = vertStream.str();
    vertexFile.close();
    const char* vertexShaderSource = vertexCode.c_str();

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shader source string has \0 to terminate, no need for providing length
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << errorLog;
    }

    // fragment shader
    fragFile.open(fragmentFilePath);
    fragStream << fragFile.rdbuf();
    fragCode = fragStream.str();
    fragFile.close();
    const char* fragmentShaderSource = fragCode.c_str();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Shader source string has \0 to terminate, no need for providing length
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << errorLog;
    }

    // link shaders to main program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);

    // Now that main shader program was made, we can delete the shader components
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(this->ID);
}

void Shader::use() {
    glUseProgram(ID);
}

GLuint Shader::getID() {
    return this->ID;
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

#endif