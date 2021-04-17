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
    this->mainSkyBox = new skyBox("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader", "./resources/Skycube/");

    // Setting the arrays for buffer id values of each item
      vertexArrayID = new GLuint[objects.size()];
     vertexBufferID = new GLuint[objects.size()];
    elementBufferID = new GLuint[objects.size()];

    // Object Stuff //
    this->objects.push_back(object("./resources/cube_Borg_textured.obj","./resources/borg_texture.bmp"));
    this->objects.push_back(object("./resources/cube_Borg_textured.obj","./resources/borg_texture.bmp"));
    this->objects[1].setPosition(glm::vec3(3.0f,1.0f,-2.0f));
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
    // Setting the background color buffer, first argument specifies the buffer, second argument is 0 as only this one buffer is being modified, 
    glClearBufferfv(GL_COLOR, 0, bg_color);
    
    // Render the skybox
    this->mainSkyBox->renderSkyBox(mainCamera.getPerspective(), mainCamera.getProjection());
    
    // Using the one object shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(objectsShader->shaderID);
   
    glEnable(GL_DEPTH_TEST);
    // Render each item
    
    // Setting camera transform
    objectsShader->setMat4("camera", mainCamera.getView());
    // Setting perspective transform
    objectsShader->setMat4("perspective", mainCamera.getPerspective());
    for (int i = 0; i < objects.size(); i++) {
        // Setting the translation transform for the obejct
        objectsShader->setMat4("translation", objects[i].getTranslation());

        // Creating the scale matrix to appriopriately set the size of the object
        glm::mat4 scaleMatrix = glm::mat4x4(objects[i].getScale());
        scaleMatrix[3].w = 1.0f; // Correcting the w componenet

        // Setting scale matrix into shader
        objectsShader->setMat4("scale", scaleMatrix);
        // Setting the orientation of the object (rotating to correctly according to it's bank, heading, and pitch)
        objectsShader->setMat4("ori", objects[i].getRotation());

        // Render the object
        objects[i].renderObject(objectsShader->getVertexID(), objectsShader->getUvID());
    }
    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
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
    delete this->mainSkyBox;
}
