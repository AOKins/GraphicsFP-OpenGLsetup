#include "../headers/shader.h"
#include <fstream>
#include <iostream>

// Default constructor uses load_from_file method to load shader code
shader::shader() {
    int success;
    char errorLog[512];
    GLuint vertexID, fragmentID;

    // Creating and compiling vertex shader
    // Setting to create new vertex shader, returning id of the shader object
    vertexID = glCreateShader(GL_VERTEX_SHADER);

    const GLchar * vertex_source = load_from_file("shaders/vertex.shader");
    
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

    const GLchar * frag_source = load_from_file("shaders/fragment.shader");
    
    // Sending the source code for the fragment shader
    glShaderSource(fragmentID, 1, &frag_source, NULL);

    // Compiling the source code that has been sent
    glCompileShader(fragmentID);

    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, NULL, errorLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << errorLog;
    }

    // Creating shader program, attaching vertex and fragment shaders to be then finally linked
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexID);
    glAttachShader(shaderID, fragmentID);
    glLinkProgram(shaderID);
    // Now that shader program has been setup, can delete shader objects to clean up unused resources
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    this->cancel = false;
    setBool("cancel", this->cancel);
}

// Really simple load_from_file method to read file contents with given path
// Input: filePath - char array that gives file path to file (ASSUMED CORRECT WITH NO CHECK)
// Output: returns const char array of file contents
const char* shader::load_from_file(const char * filePath) {
    char * fileContent;
    std::ifstream fileStream;
    fileStream.open(filePath);
    int i = 0;
    // Read each character at a time, storing the result into fileContent array
    while (!fileStream.eof()) {
        fileStream.read(fileContent + i, 1);
        ++i;
    }
    // Ending with zero character to designate end of string
    fileContent[i-1] = '\0';

    fileStream.close();
    return fileContent;
}

shader::~shader() {
    glDeleteProgram(shaderID);
}


void shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value); 
}

void shader::setInt(const std::string &name, int value) const { 
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value); 
}

void shader::setFloat(const std::string &name, float value) const { 
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value); 
}

void shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
