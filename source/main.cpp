#include <iostream>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include "../headers/vertex.h"
#include "../headers/color.h"
#include "../headers/vertexture.h"
#include "../headers/shader.h"
#include "../headers/camera.h"
#include "callbacks.cpp"

// The default window dimensions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Initialize the libraries and creating window, returns pointer to the window
GLFWwindow* setup() {
    // Setting things up //
    // Initializing the GLFW library
    if (!glfwInit()) {
        std::cout << "failed to initialize GLFW!\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    // Disable the ability to resize the window generated
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Our initial main window dimensions is going to be what is defined
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"My Window", NULL, NULL);


    // If failed to create the window object, exit now
    if (!window) {
        std::cout << "uhh, window failed to be created\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Initializing the GLEW library (returning 0 means no errors)
    if (glewInit()) {
        std::cout << "GLEW failed to initialize!\n";
        exit(EXIT_FAILURE);
    }

    return window;
}

int main() {
    // Setup the window with current context
    GLFWwindow * window = setup();

    Camera myCamera(window);

    // Setting the initial GL render window to be the entire window
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    // Setting key callback to method
    glfwSetKeyCallback(window, key_callback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Outputting onto the terminal the version of OpenGl being used
    std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << std::endl;

    // Setup the shader application using the shader class created
    Shader myShader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");

    // Scene Data //
    // Vertices of the object with color and texture coordinates
    vertexture vertices[] = {
        // Front             x      y     z
        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
        vertexture( vertex( 0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex( 0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex( 0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex(-0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),
        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),

        vertexture( vertex(-0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
        vertexture( vertex( 0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex(-0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),
        vertexture( vertex(-0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),

        vertexture( vertex(-0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
        vertexture( vertex(-0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f), 
        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f), 
        vertexture( vertex(-0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
        vertexture( vertex(-0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f),

        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  1.0f, 0.0f),
        vertexture( vertex( 0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  1.0f, 1.0f), 
        vertexture( vertex( 0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  0.0f, 1.0f), 
        vertexture( vertex( 0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  0.0f, 1.0f), 
        vertexture( vertex( 0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  0.0f, 0.0f),
        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f),  1.0f, 0.0f),

        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),
        vertexture( vertex( 0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex( 0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex( 0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex(-0.5f, -0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
        vertexture( vertex(-0.5f, -0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),

        vertexture( vertex(-0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),
        vertexture( vertex( 0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f), 
        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex( 0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.0f), 
        vertexture( vertex(-0.5f,  0.5f,  0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f),
        vertexture( vertex(-0.5f,  0.5f, -0.5f), color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 1.0f),
    };
    // positions of cubes using the vertices array to define each cube
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    // TEXTURE STUFF //
    int tex_width, tex_height, tex_nrChannels;
    unsigned int textureID0,textureID1;
    // FIRST TEXTURE LOADING

    glGenTextures(1, &textureID0);
    glBindTexture(GL_TEXTURE_2D, textureID0);

    unsigned char * tex_data0 = stbi_load("resources/container.jpg", &tex_width, &tex_height, &tex_nrChannels, 0);
    if (tex_data0) {
        // Loading the texture image data //
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data0);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::STBI_LOAD::FAILURE_TO_LOAD_TEXTURE_AT::\"resources/wall.jpg\"\n";
    }

    stbi_image_free(tex_data0); // No longer need that data on this end
    
    // SECOND TEXTURE LOADING

    glGenTextures(1, &textureID1);
    glBindTexture(GL_TEXTURE_2D, textureID1);

    stbi_set_flip_vertically_on_load(true);
    unsigned char * tex_data1 = stbi_load("resources/awesomeface.png", &tex_width, &tex_height, &tex_nrChannels, 0);

    if (tex_data1) {
        // Loading the texture image data //
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::STBI_LOAD::FAILURE_TO_LOAD_TEXTURE_AT::\"resources/awesomeface.png\"\n";
    }
    stbi_image_free(tex_data1); // No longer need that data on this end

    // End of texture image load // 

    // Generate the data buffers
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the buffers, not that VBO is to the array buffer and EBO to the element array
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Send the data over
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setting the attributes for the position of vertices (3 components so 3 in second argument)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexture), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Setting the attributes for the colors of vertices (4 values so 4 in second argument)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertexture), (GLvoid*)(sizeof(vertex)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexture), (GLvoid*)(sizeof(vertex) + sizeof(color)));
    glEnableVertexAttribArray(2);

    // Using the shader once to initialize before setting values
    myShader.use();
    // Setting the values for location of the two textures in the shaders
    myShader.setInt("texture0", 0);
    myShader.setInt("texture1", 1);

    // Creating and setting up the camera
    
    myCamera.setPerspective(90.0f);

    // Initialize the 3 matrices needed
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);


    projection = glm::perspective(myCamera.getPerspective(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    float lastTime, currentTime, dTime;
    lastTime = 0.0f;
    currentTime = 0.0f;
    dTime = 0.0f;
    // The render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // Setting background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Bind the textures to texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID1);


        myShader.setMat4("model", model);
        myShader.setMat4("view", myCamera.getView());
        myShader.setMat4("projection", projection);

        // Render the window
        myShader.use();
        glBindVertexArray(VAO);

        // Uses the cube positions to create a scene, drawing each one
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i % 10]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));

            myShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/ sizeof(vertexture));
        }
        // Swap the buffers to update window display
        glfwSwapBuffers(window);

        currentTime = glfwGetTime();
        dTime = currentTime - lastTime;
        lastTime = currentTime;
        // Call camera's input update, where it reads for key input
        myCamera.inputUpdate(dTime);

        // Process event
        glfwPollEvents();
    }

    // Closing window and application //
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
