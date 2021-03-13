#include "../headers/app.h"
#include "shader.cpp"
#include "input.cpp"
#include "transformDerive.cpp"
#include <iostream>

// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    this->show_inverse = false;
    this->show_projection = false;
    // Default screen resolution is 760x480
    this->window_width = 760;
    this->window_height = 760;
    this->num_triangles = 12; // 2 triangles for each side of a 6 sided cube
    this->triangleData = new vertexColor[3*this->num_triangles];
    this->transformMode = NONE;

    this->pos_x = 0;
    this->pos_y = 0;
    this->pos_z = 0;
    this->scaleSize = 1.0f;

    // Big (and nasty) series of assignments to initialize the triangleData to form a cube //
    // Setting positions and colors, with color of each side being unique with lighter and darker triangles //
    // Front of Cube
    triangleData[0] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[1] = vertexColor(
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4( 1.0f,  0.0f, 0.0f, 1.0f)
    );
    triangleData[2] = vertexColor(
        glm::vec4( 0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4( 1.0f,  0.0f, 0.0f, 1.0f)
    );
    triangleData[3] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[4] = vertexColor(
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4( 0.5f,  0.0f, 0.0f, 1.0f)
    );
    triangleData[5] = vertexColor(
        glm::vec4(-0.5f,  0.5f, 0.5f, 1.0f),
        glm::vec4( 0.5f,  0.0f, 0.0f, 1.0f)
    );
    // Back
    triangleData[6] = vertexColor(
        glm::vec4( 0.5f, 0.5f, -0.5f, 1.0f), // Position
        glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[7] = vertexColor(
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4( 0.0f,  1.0f, 0.0f, 1.0f)
    );
    triangleData[8] = vertexColor(
        glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4( 0.0f,  1.0f, 0.0f, 1.0f)
    );
    triangleData[9] = vertexColor(
        glm::vec4( 0.5f, 0.5f, -0.5f, 1.0f), // Position
        glm::vec4( 0.0f, 0.5f, 0.0f, 1.0f)   // Color
    );
    triangleData[10] = vertexColor(
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4( 0.0f,  0.5f, 0.0f, 1.0f)
    );
    triangleData[11] = vertexColor(
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4( 0.0f, 0.5f, 0.0f, 1.0f)
    );
    // Right
    triangleData[12] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[13] = vertexColor(
        glm::vec4(0.5f,-0.5f,  -0.5f, 1.0f),
        glm::vec4( 1.0f,  1.0f, 0.0f, 1.0f)
    );
    triangleData[14] = vertexColor(
        glm::vec4( 0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4( 1.0f,  1.0f, 0.0f, 1.0f)
    );
    triangleData[15] = vertexColor(
        glm::vec4( 0.5f, 0.5f, -0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.5f, 0.0f, 1.0f)   // Color
    );
    triangleData[16] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f)
    );
    triangleData[17] = vertexColor(
        glm::vec4( 0.5f, -0.5f,-0.5f, 1.0f),
        glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f)
    );
    // Left
    triangleData[18] = vertexColor(
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f)   // Color
    );
    triangleData[19] = vertexColor(
        glm::vec4(-0.5f,-0.5f,  -0.5f, 1.0f),
        glm::vec4( 0.0f,  0.0f, 1.0f, 1.0f)
    );
    triangleData[20] = vertexColor(
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4( 0.0f,  0.0f, 1.0f, 1.0f)
    );
    triangleData[21] = vertexColor(
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f), // Position
        glm::vec4( 0.0f, 0.0f, 0.5f, 1.0f)   // Color
    );
    triangleData[22] = vertexColor(
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.0f,  0.0f, 0.5f, 1.0f)
    );
    triangleData[23] = vertexColor(
        glm::vec4(-0.5f, -0.5f,-0.5f, 1.0f),
        glm::vec4( 0.0f,  0.0f, 0.5f, 1.0f)
    );
    // Bottom
    triangleData[24] = vertexColor(
        glm::vec4(-0.5f,-0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)   // Color
    );
    triangleData[25] = vertexColor(
        glm::vec4(-0.5f,-0.5f,-0.5f, 1.0f),
        glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f)
    );
    triangleData[26] = vertexColor(
        glm::vec4( 0.5f, -0.5f,-0.5f, 1.0f),
        glm::vec4( 1.0f,  1.0f, 1.0f, 1.0f)
    );
    triangleData[27] = vertexColor(
        glm::vec4(0.5f,-0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f)   // Color
    );
    triangleData[28] = vertexColor(
        glm::vec4(0.5f,-0.5f,-0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f)
    );
    triangleData[29] = vertexColor(
        glm::vec4(-0.5f,-0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f)
    );
    // Top
    triangleData[30] = vertexColor(
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f)   // Color
    );
    triangleData[31] = vertexColor(
        glm::vec4(-0.5f, 0.5f,-0.5f, 1.0f),
        glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f)
    );
    triangleData[32] = vertexColor(
        glm::vec4( 0.5f, 0.5f,-0.5f, 1.0f),
        glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f)
    );
    triangleData[33] = vertexColor(
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4(0.5f, 0.0f, 0.5f, 1.0f)   // Color
    );
    triangleData[34] = vertexColor(
        glm::vec4(0.5f, 0.5f,-0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.0f, 0.5f, 1.0f)
    );
    triangleData[35] = vertexColor(
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f), // Position
        glm::vec4( 0.5f, 0.0f, 0.5f, 1.0f)
    );

    // Call initialize
    initialize();
}

// Sets up the window and context, also initialization of GLEW
void application::initialize() {
    // Create window for app
    this->window = SDL_CreateWindow("Main Window", 10, 100, window_width, window_height, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("ERROR::APPLICATION\nDid not successfully create window\n");
        exit(EXIT_FAILURE);
    }

    // Creating context for window
    this->context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        printf("ERROR::APPLICATION\nDid not successfully create context\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize glew library
    if (glewInit() != GLEW_OK) {
        printf("ERROR::APPLICATION\nFailed to initialize GLEW\n");
        exit(EXIT_FAILURE);
    }
    // Setting viewport to be same as window itself
    glViewport(0, 0, window_width, window_height);

    // Creating default shader (uses files in /shaders folder)
    shaderApp = new shader();
}

// Start the application, also calls
void application::start() {
    this->running = true;

    // Setting up the triangle info //
    // Creating one vertex array, setting an id to vertexArrayID
    glCreateVertexArrays(1, &vertexArrayID);
    // Creating a couple of buffers for the data, attaching ids to variables
    glCreateBuffers(1, &vertexBufferID);
    glCreateBuffers(1, &elementBufferID);
    // Setting the vertex array and buffers to be binded to this current running context, arguments specify which buffer type and id
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    
    // Send triangle data to the buffer, specifing that it is to the array buffer, providing size and address, followed by the usage (which here is as static drawing)
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(vertexColor)*num_triangles, triangleData, GL_STATIC_DRAW);

    // Setting attributes to the vertex array so that it knows how to uses the vertex array
    // resource that was used in this usage and acquirng this current state of understanding is https://learnopengl.com/Getting-started/Hello-Triangle
    // first argument setting input variable being attributed which is zero as the location for the vertices data is set to 0 in the shader (location = 0 for position)
    // second argument specifies number of values (the vertex data is comprised of 4 data points as well as color)
    // third argument gives size of each value (floats)
    // fourth sets to normalize the data if true
    // fifth argument gives the distance between each set of data
    // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertexColor), (GLvoid*)0);

    // Enabling the arrays that have been created to be used in the vertex shader
    glEnableVertexAttribArray(0);
    // Setting attribute for color info at location 1, have to offset by vector of position data
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertexColor), (GLvoid*)(sizeof(glm::vec4)) );
    glEnableVertexAttribArray(1);
    // Enabling the depth buffer since dealing with depth now
    glEnable(GL_DEPTH_TEST);

    // Call the loop method to 
    loop();
}

// Core event/render loop
void application::loop() {
    SDL_Event event;
    double currTime;
    double lastTime;
    // Last time is currently now
    lastTime = double(SDL_GetTicks()) / 1000.0f;
    while (this->running) {
        // Get current time in milliseconds
        currTime = double(SDL_GetTicks()) / 1000.0f;
        while(SDL_PollEvent(&event)) {
            this->event(&event);
        }
        render(currTime, lastTime);
        // Before entering new loop, this time is now last time
        lastTime = currTime;
    }
}

// Handles the actual rendering behavior (including manage of triangle data)
void application::render(double ctime, double ltime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Setting the background color buffer, first argument specifies the buffer, second argument is 0 as only this one buffer is being modified, 
    glClearBufferfv(GL_COLOR, 0, bg_color);

    glm::mat4x4 transformMatrix, scale, translate, projection, perspective, rotation, inverse;

          scale =       getScale(ctime);
       rotation =    getRotation(ctime);
      translate =   getTranslate(ctime);
    perspective = getPerspective(ctime);

    transformMatrix =  scale * rotation * translate;

    // Applying transform matrix to the shader
    shaderApp->setMat4("transform", transformMatrix * perspective);

    // Updating the buffer data
    // First argument specifies that this is an array
    // Second argument gives size of the array
    // Third argument gives start of the array via the array's address
    // Fourth argument tells that this data is to be accessed and modified very often 
    //      (https://www.reddit.com/r/opengl/comments/57i9cl/examples_of_when_to_use_gl_dynamic_draw/ was used to find this, as I had prior used GL_STATIC_DRAW but wanted to know what other options I had that would be more applicable)
    glBufferData(GL_ARRAY_BUFFER, 3*num_triangles*sizeof(vertexColor), triangleData, GL_DYNAMIC_DRAW);
    
    // Now dealing with rendering the triangle //
    // Using the one shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(shaderApp->shaderID);

    // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
    glBindVertexArray(vertexArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 3*num_triangles);

    // Show the projection of the result on the y plane (enable wireframe to see)
    if (this->show_projection) {
        projection = transformMatrix * getProjection(ctime);

        // Applying transform matrix to the shader
        shaderApp->setMat4("transform", projection);

        // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
        glBindVertexArray(vertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, 3*num_triangles);
    }

    // Show an in
    if (this->show_inverse) {
        inverse = getInverse(scale, rotation, translate);

        // Applying transform matrix to the shader
        shaderApp->setMat4("transform",inverse * transformMatrix );


        // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
        glBindVertexArray(vertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, 3*num_triangles);
    }

    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
    glDeleteVertexArrays(1, &vertexArrayID);
    // Closing window and closing library
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Simple getter for running property
bool application::isRunning(){
    return this->running;
}

// Simple deconstructor that deletes the shader and triangle data
application::~application() {
    delete this->shaderApp;
    delete [] this->triangleData;
}
