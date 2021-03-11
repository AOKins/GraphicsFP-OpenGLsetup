#include "../headers/app.h"
#include "shader.cpp"
#include "input.cpp"
#include <iostream>

// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    // Default screen resolution is 760x480
    this->window_width = 760;
    this->window_height = 760;
    this->num_triangles = 2;
    this->triangleData = new vertexColor[3*this->num_triangles];
    this->transformMode = NONE;

    // Front bottom right triangle
    triangleData[0] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.0f, 1.0f), // Position
        glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[1] = vertexColor(
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4( 1.0f,  0.0f, 0.0f, 1.0f)
    );
    triangleData[2] = vertexColor(
        glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4( 1.0f,  0.0f, 0.0f, 1.0f)
    );

    // Front bottom right triangle
    triangleData[3] = vertexColor(
        glm::vec4( 0.5f, 0.5f, 0.0f, 1.0f), // Position
        glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f)   // Color
    );
    triangleData[4] = vertexColor(
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4( 0.0f,  1.0f, 0.0f, 1.0f)
    );
    triangleData[5] = vertexColor(
        glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f),
        glm::vec4( 0.0f,  1.0f, 0.0f, 1.0f)
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
    glewInit();
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
    
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertexColor), (GLvoid*)(4*sizeof(float)) );
    glEnableVertexAttribArray(1);
    
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

    double delta = ctime - ltime;
    glm::mat4x4 transformMatrix;
    float scaleSize = 1.2f;

    switch(transformMode) {
        case (TRANSLATION):
            // Having translation be dependent on time with trig methods to have overall behavior be circular motion
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, glm::cos(ctime)/2.0f,
                0.0f, 1.0f, 0.0f, glm::sin(ctime)/2.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case(SCALE):
            transformMatrix = glm::mat4x4(
                scaleSize, 0.0f,      0.0f,   0.0f,
                0.0f, scaleSize,      0.0f,   0.0f,
                0.0f,      0.0f, scaleSize,   0.0f,
                0.0f,      0.0f,      0.0f,   1.0f);
            break;
        case (ROTATION):
            transformMatrix = glm::mat4x4(
                 glm::cos(ctime/2.0f), glm::sin(ctime/2.0f), 0.0f, 0.0f,
                -glm::sin(ctime/2.0f), glm::cos(ctime/2.0f), 0.0f, 0.0f,
                            0.0f,            0.0f, 1.0f, 0.0f,
                            0.0f,            0.0f, 0.0f, 1.0f);
            break;
        case(REFLECTION):
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f,-1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
            break;
        case(SHEARING):
            transformMatrix = glm::mat4x4(
                1.4f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.6f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case(INVERSION): // Currently not implemented
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case(PROJECTION): // Currently not implemented
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case(FUN): // Currently not implemented
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case(RESET): // Not implemented
            break;
        case(NONE): // Don't do anything
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
    }
    // Applying transform matrix if not in reset mode or none
    shaderApp->setMat4("transform",transformMatrix);

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

bool application::isRunning(){
    return this->running;
}

application::~application() {
    delete this->shaderApp;
    delete [] this->triangleData;
}
