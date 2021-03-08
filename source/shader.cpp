#include "../headers/shader.h"
#include <fstream>
#include <iostream>

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
    
    std::cout << frag_source << std::endl;

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
}

const char* shader::load_from_file(const char * filePath) {
    char * fileContent;
    std::ifstream fileStream;

    fileStream.open(filePath);

    int i = 0;
    while (!fileStream.eof()) {
        fileStream.read(fileContent + i, 1);

        ++i;
    }
    fileContent[i-1] = '\0';

    fileStream.close();


    return fileContent;
}
