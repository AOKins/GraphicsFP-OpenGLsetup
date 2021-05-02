#include "../headers/shader.h"
#include <fstream>
#include <iostream>
#include <type_traits>

shader::shader(const char * vertexName, const char * fragmentName) {
    int success;
    char errorLog[512];
    GLuint vertexID, fragmentID;

    // Creating and compiling vertex shader
    // Setting to create new vertex shader, returning id of the shader object
    vertexID = glCreateShader(GL_VERTEX_SHADER);

    const GLchar * vertex_source = load_from_file(vertexName);
    
    // Sending the source code for the vertex shader
    glShaderSource(vertexID, 1, &vertex_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << errorLog;
    }

    // Creating and compiling fragment shader
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar * frag_source = load_from_file(fragmentName);
    
    // Sending the source code for the fragment shader
    glShaderSource(fragmentID, 1, &frag_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(fragmentID);

    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << errorLog;
    }

    // Creating shader program, attaching vertex and fragment shaders to be then finally linked
    this->shaderID = glCreateProgram();
    glAttachShader(this->shaderID, vertexID);
    glAttachShader(this->shaderID, fragmentID);
    glLinkProgram(this->shaderID);
    // Now that shader program has been setup, can delete shader objects to clean up unused resources
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    this->cancel = false;
}

// Really simple load_from_file method to read file contents with given path
// Input: filePath - char array that gives file path to file (ASSUMED CORRECT WITH NO CHECK)
// Output: returns const char array of file contents
const char* shader::load_from_file(const char * filePath) {
    char * fileContent;
    FILE * file;
    size_t size;
    file = fopen(filePath, "rb");

    if (!file) return 0;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file,0,SEEK_SET);    
    fileContent = new char [size + 1];

    if (!fileContent) return 0;

    fread(fileContent, 1, size, file);
    fileContent[size] = 0;
    fclose(file);

    return fileContent;
}

shader::~shader() {
    glDeleteProgram(shaderID);
}

// Implementing the set methods for uniform variables
// Based on example from learnopengl.com 
// Bool uniform
void shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value); 
}

// Integer uniform
void shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value); 
}

// Float uniform
void shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value); 
}

void shader::setNfloat(const std::string &name, int n, float &start) const { 
    glUniform1fv(glGetUniformLocation(shaderID, name.c_str()), n, &start); 
}

void shader::setNvec3(const std::string &name, int n, glm::vec3 &start) const {
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), n, glm::value_ptr(start)); 
}

void shader::setNvec4(const std::string &name, int n, glm::vec4 &start) const {
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), n, glm::value_ptr(start)); 
}

void shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, glm::value_ptr(value)); 
}

void shader::setVec4(const std::string &name, glm::vec4 value) const {
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, glm::value_ptr(value)); 
}

template<typename T>
void shader::setNvalue(const std::string &name, int n, T &start) const {
    glUniform1fv(glGetUniformLocation(shaderID, name.c_str()),n, start);
}


// 4x4 matrix uniform
void shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// Getter for attribute location of given name
GLuint shader::getLocation(const std::string &name) {
    return glGetAttribLocation(this->shaderID, name.c_str());
}

