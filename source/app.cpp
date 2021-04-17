#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <iostream>

#include "../headers/app.h"
#include "shader.cpp"
#include "input.cpp"
#include "object.cpp"
#include "camera.cpp"
#include "./functions/transformDerive.cpp"

#include "skyBox.cpp"


#define GL_CHECK_ERR assert(glGetError() == GL_NO_ERROR);

// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    // Default screen resolution is 
    this->window_width = 1352;
    this->window_height = 760;

    // Call initialize
    initialize();
}

// Sets up the window and context, also initialization of GLEW and SDL
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

    // Creating shaders for objects and skyCube (uses files in /shaders folder)
    objectsShader = new shader("./shaders/vertex.shader","./shaders/fragment.shader");
    skyCubeShader = new shader("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader");
}

// Start the application, ending with call to loop()
void application::start() {
    this->running = true;
    // Correcting main camera to the aspect ratio of the window
    mainCamera.setAspect(float(window_width)/float(window_height));

    // SkyBox //
    this->mainSkyBox = new skyBox("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader");

    // Object Stuff //
    this->objects.push_back(object("./resources/test_cube.obj"));

    // Setting the arrays for buffer id values of each item
      vertexArrayID = new GLuint[objects.size()];
     vertexBufferID = new GLuint[objects.size()];
    elementBufferID = new GLuint[objects.size()];

    // Creating one vertex array, setting an id to vertexArrayID
    glCreateVertexArrays(objects.size(), vertexArrayID);

    // Creating a couple of buffers for the data, attaching ids to variables
    glCreateBuffers(objects.size(), vertexBufferID);
    glCreateBuffers(objects.size(), elementBufferID);
    
    // Setting up the triangle info for objects //
    for (int i = 0; i < objects.size(); i++) {
        // Setting the vertex array and buffers to be binded to this current running context, arguments specify which buffer type and id
        glBindVertexArray(vertexArrayID[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID[i]);

        // Send triangle data to the buffer, specifing that it is to the array buffer, providing size and address, followed by the usage (which here is as static drawing)
        glBufferData(GL_ARRAY_BUFFER, objects[i].vertices.size() * sizeof(objects[i].vertices[0]), objects[i].vertices.data() , GL_STATIC_DRAW);

        // Setting attributes to the vertex array so that it knows how to uses the vertex array
        // resource that was used in this usage and acquirng this current state of understanding is https://learnopengl.com/Getting-started/Hello-Triangle
        // first argument setting input variable being attributed which is zero as the location for the vertices data is set to 0 in the shader (location = 0 for position)
        // second argument specifies number of values (the vertex data is comprised of 4 data points as well as color)
        // third argument gives size of each value (floats)
        // fourth sets to normalize the data if true
        // fifth argument gives the distance between each set of data
        // sixth gives offset in the buffer to start off with (which is 0 as there is no need for offsetting) 
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        // Enabling the arrays that have been created to be used in the vertex shader
        glEnableVertexAttribArray(0);
    }
    // End of Object Stuff //

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
            this->event(&event, currTime - lastTime);
        }
        render(currTime, lastTime);
        // Before entering new loop, this time is now last time
        lastTime = currTime;
    }
}

// Handles the actual rendering behavior (including manage of triangle data)
void application::render(double ctime, double ltime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // Setting the background color buffer, first argument specifies the buffer, second argument is 0 as only this one buffer is being modified, 
    glClearBufferfv(GL_COLOR, 0, bg_color);
    
    // Render the skybox
    this->mainSkyBox->renderSkyBox(mainCamera.getPerspective(), mainCamera.getProjection());
    
    // Using the one object shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(objectsShader->shaderID);

    // Render each item
    for (int i = 0; i < objects.size(); i++) {
        // Updating the buffer data
        // First argument specifies that this is an array
        // Second argument gives size of the array
        // Third argument gives start of the array via the array's address
        // Fourth argument tells that this data is to be accessed and modified very often 
        //      (https://www.reddit.com/r/opengl/comments/57i9cl/examples_of_when_to_use_gl_dynamic_draw/ was used to find this, as I had prior used GL_STATIC_DRAW but wanted to know what other options I had that would be more applicable)
        glBufferData(GL_ARRAY_BUFFER, objects[i].vertices.size() * sizeof(objects[i].vertices[i]), objects[i].vertices.data(), GL_DYNAMIC_DRAW);

        // Setting camera transform
        objectsShader->setMat4("camera", mainCamera.getView());
        // Setting perspective transform
        objectsShader->setMat4("perspective", mainCamera.getPerspective());

        // Setting the translation transform for the obejct
        objectsShader->setMat4("translation", objects[i].getTranslation());

        // Creating the scale matrix to appriopriately set the size of the object
        glm::mat4 scaleMatrix = glm::mat4x4(objects[i].getScale());
        scaleMatrix[3].w = 1.0f; // Correcting the w componenet

        // Setting scale matrix into shader
        objectsShader->setMat4("scale", scaleMatrix);
        // Setting the orientation of the object (rotating to correctly according to it's bank, heading, and pitch)
        objectsShader->setMat4("ori", objects[i].getRotation());

        // Now dealing with actively rendering the triangles //

        // Call to draw the tirangle, starting at index 0 and number of vertices to render for triangles (using size of entire array divided by size of each vertex)
        glBindVertexArray(vertexArrayID[i]);
        glDrawArrays(GL_TRIANGLES, 0, objects[i].vertices.size());
    }
    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
    // Delete the object arrays in the buffers
    glDeleteVertexArrays(objects.size(), vertexArrayID);
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
    // Clear up everything
    delete this->objectsShader;
    delete this->skyCubeShader;
    delete [] vertexArrayID;
    delete [] vertexBufferID;
    delete [] elementBufferID;
    delete this->mainSkyBox;
}
