#include "../headers/app.h"
#include "shader.cpp"
#include "input.cpp"
#include <iostream>
// #include "shader.cpp"

// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    // Default screen resolution is 760x480
    this->window_width = 760;
    this->window_height = 760;
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
    
    // Send triangelVertices data to the buffer, specifing that it is to the array buffer, providing size and address, followed by the usage (which here is as static drawing)
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices, GL_STATIC_DRAW);

    // Setting attributes to the vertex array so that it knows how to uses the vertex array
    // resource that was used in this usage and acquirng this current state of understanding is https://learnopengl.com/Getting-started/Hello-Triangle
    // first argument setting input variable being attributed which is zero as the location for the vertices data is set to 0 in the shader (location = 0 for position)
    // second argument specifies number of values (the vertex data is comprised of 3 data points)
    // third argument gives size of each value (floats)
    // fourth sets to normalize the data if true
    // fifth argument gives the distance between each set of data, since it's a homogenous series of vertices its just the size of a vertex (3 floats)
    // (if there was data in-between we would need to take that into account)
    // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)0);

    // Enabling the array that has been created to be used in the vertex shader
    glEnableVertexAttribArray(0);
    
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
            transformMatrix = glm::mat4x4(
                1.0f, 0.0f, 0.0f, glm::cos(ctime)/200.0f,
                0.0f, 1.0f, 0.0f, glm::sin(ctime)/200.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            triangleVertices[0] = triangleVertices[0] * transformMatrix;
            triangleVertices[1] = triangleVertices[1] * transformMatrix;
            triangleVertices[2] = triangleVertices[2] * transformMatrix;
            break;
        case(SCALE):
            transformMatrix = glm::mat4x4(
                scaleSize, 0.0f,      0.0f,   0.0f,
                0.0f, scaleSize,      0.0f,   0.0f,
                0.0f,      0.0f, scaleSize,   0.0f,
                0.0f,      0.0f,      0.0f,   1.0f);
            triangleVertices[0] = triangleVertices[0] * transformMatrix;
            triangleVertices[1] = triangleVertices[1] * transformMatrix;
            triangleVertices[2] = triangleVertices[2] * transformMatrix;
            transformMode = NONE;
            break;
        case (ROTATION):
            transformMatrix = glm::mat4x4(
                 glm::cos(delta), glm::sin(delta), 0.0f, 0.0f,
                -glm::sin(delta), glm::cos(delta), 0.0f, 0.0f,
                            0.0f,            0.0f, 1.0f, 0.0f,
                            0.0f,            0.0f, 0.0f, 1.0f
            );
            triangleVertices[0] = triangleVertices[0] * transformMatrix;
            triangleVertices[1] = triangleVertices[1] * transformMatrix;
            triangleVertices[2] = triangleVertices[2] * transformMatrix;
            break;
        case(REFLECTION):
            break;
        case(SHEARING):
            transformMatrix = glm::mat4x4(
                1.2f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.8f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            triangleVertices[0] = triangleVertices[0] * transformMatrix;
            triangleVertices[1] = triangleVertices[1] * transformMatrix;
            triangleVertices[2] = triangleVertices[2] * transformMatrix;
            transformMode = NONE;
            break;
        case(INVERSION):
            break;
        case(PROJECTION):
            break;
        case(FUN):
            std::cout << "X,\tY,\tZ,\tW\n";
            std::cout << triangleVertices[0].x << "\t" << triangleVertices[0].y << "\t" << triangleVertices[0].z << "\t" << triangleVertices[0].w << "\n";
            std::cout << triangleVertices[1].x << "\t" << triangleVertices[1].y << "\t" << triangleVertices[1].z << "\t" << triangleVertices[1].w << "\n";
            std::cout << triangleVertices[2].x << "\t" << triangleVertices[2].y << "\t" << triangleVertices[2].z << "\t" << triangleVertices[2].w << "\n";
            transformMode = NONE;
            break;
        case(NONE):
            break;
    }

    // Updating the buffer data by calling glBufferData again with array
    // First argument specifies that this is an array
    // Second argument gives size of the array
    // Third argument gives start of the array via the array's address
    // Fourth argument tells that this data is to be accessed and modified very often 
    //      (https://www.reddit.com/r/opengl/comments/57i9cl/examples_of_when_to_use_gl_dynamic_draw/ was used to find this, as I had prior used GL_STATIC_DRAW but wanted to know what other options I had that would be more applicable)
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), &triangleVertices, GL_DYNAMIC_DRAW);
    
    // Now dealing with rendering the triangle //
    // Using the one shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(shaderApp->shaderID);

    // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
    glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleVertices)/(sizeof(glm::vec3)));

    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
    
    glDeleteVertexArrays(1, &vertexArrayID);
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool application::isRunning(){
    return this->running;
}

