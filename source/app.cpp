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

#include "./functions/graphicsMethods.cpp"

// Default constructor, calls initialize and sets running to false
application::application() {
    this->running = false;
    // Default screen resolution is 
    this->window_width = 1352;
    this->window_height = 760;
    // Set dimension for shadow map
    this->shadowRes = 3000;

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

}

// Start the application, ending with call to loop()
void application::start() {
    this->running = true;
    // Correcting main camera to the aspect ratio of the window
    mainCamera.setAspect(float(window_width)/float(window_height));

    // Creating shaders (uses files in ./shaders folder)
    // Objects //
    this->objectsShader = new shader("./shaders/vertex.shader","./shaders/fragment.shader");
    // SkyBox //
    this->mainSkyBox = new skyBox("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader", "./resources/Skycube/");
    // Shadow Mapping //
    this->shadowMapShader = new shader("./shaders/shadow_vertex.shader","./shaders/shadow_fragment.shader");

    // Object Stuff //
    // Demo spheres to give more reference points to show light effects
    this->objects.clear();
    this->elementBuffers.clear();
    this->vertexArrays.clear();
    this->vertexBuffers.clear();
    this->normalArrays.clear();
    this->normalBuffers.clear();
    this->uvBuffers.clear();
    this->textureIDs.clear();

    // Load objects
    this->objects.push_back(object("./resources/plane.obj"));
    this->objects.push_back(object("./resources/simple_car.obj"));

    // Load textures
    this->textureIDs.push_back(GLmethods::load_texture("./resources/grass.bmp"));
    this->textureIDs.push_back(GLmethods::load_texture("./resources/test_white.bmp"));

    // Reserving space
    this->elementBuffers.reserve(this->objects.size());
    this->vertexArrays  .reserve(this->objects.size());
    this->vertexBuffers .reserve(this->objects.size());
    this->normalArrays  .reserve(this->objects.size());
    this->normalBuffers .reserve(this->objects.size());
    this->uvBuffers     .reserve(this->objects.size());

    // Setting textures for objects
    this->objects[0].setTextureID(this->textureIDs[0]);
    this->objects[1].setTextureID(this->textureIDs[1]);

    this->objects[0].setScale(0.5f);
    this->objects[0].setPosition(glm::vec3(0,-2.5,0));
    this->objects[1].setPosition(glm::vec3(0,0.5,0));

    // Load the objects into OpenGL
    for (int i = 0; i < this->objects.size(); i++) {
        GLmethods::load_object(this->objectsShader, &this->objects[i], &elementBuffers[i], &vertexArrays[i], 
                                 &vertexBuffers[i],  &normalArrays[i],  &normalBuffers[i],    &uvBuffers[i]);
    }

    // Light Setup
    this->lightPos.clear();
    this->lightColors.clear();
    this->lightIntensities.clear();
    this->lightDiffuse.clear();
    this->lightAmbient.clear();
    this->lightSpecular.clear();
    this->lightAlpha.clear();

    // Setting up Depth Map for Shadow mapping //
    GLmethods::setupDepthMap(this->depthMapBuffer, this->depthMapTexture, this->shadowRes);

    this->lightPos.push_back(glm::vec3(2,4,1));
    this->lightColors.push_back(glm::vec3(1.0,1.0,1.0));
    this->lightIntensities.push_back(8000);
    this->lightDiffuse.push_back(0.95);
    this->lightAmbient.push_back(0.15);
    this->lightSpecular.push_back(0.05);
    this->lightAlpha.push_back(2);

    // Call the loop method to 
    loop();
}

// Core event/render loop
void application::loop() {
    SDL_Event event;
    double currTime,lastTime;
    // Last time is currently now
    lastTime = double(SDL_GetTicks()) / 1000.0f;
    while (this->running) {
        // Get current time in milliseconds
        currTime = double(SDL_GetTicks()) / 1000.0f;
        // Check for keyboard inputs that we want continous (like movement)
        this->continuousKeyInput(currTime - lastTime);
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
    // Having only light at index 0 have the shadow
    glm::mat4 lightView,lightSpaceMatrix;
    glm::mat4 lightProj = glm::ortho(-100.0f,100.0f,-100.0f,100.0f,0.1f,157.5f);
    lightView = glm::lookAt(lightPos[0] + mainCamera.getPosition(), glm::vec3(0)+ mainCamera.getPosition(),glm::vec3(0,0,1));
    lightSpaceMatrix = lightProj * lightView;

    // Render the shadow mappings for all the objects //
    glUseProgram(this->shadowMapShader->shaderID);
    shadowMapShader->setMat4("lightSpace", lightSpaceMatrix);
    
    glViewport(0,0, this->shadowRes, this->shadowRes);
    glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);    // Clearing the frame buffer
    for (int i = 0; i < objects.size(); i++) {
        GLmethods::render_shadow(this->shadowMapShader, &this->objects[i], this->vertexBuffers[i],
                                this->depthMapBuffer, this->depthMapTexture, this->shadowRes);
    }

    // Render the scene normally //
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0, this->window_width, this->window_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
        // Using the one object shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(objectsShader->shaderID);

    int numLights = lightPos.size();
    if (numLights > 0) {
        objectsShader->setInt("LightCount", numLights);
        objectsShader->setNvec3("Lpos", numLights, lightPos[0]);
        objectsShader->setNvec3("Lcolor", numLights, lightColors[0]);
        objectsShader->setNfloat("Linten",numLights, lightIntensities[0]);
        objectsShader->setNfloat("Kambient", numLights, lightAmbient[0]);
        objectsShader->setNfloat("Kspecular",numLights, lightSpecular[0]);
        objectsShader->setNfloat("Kdiffuse", numLights, lightDiffuse[0]);
        objectsShader->setNfloat("U_alpha", numLights, lightAlpha[0]);
    }

    // Setting camera transform
    objectsShader->setMat4("camera", mainCamera.getView());
    // Setting perspective transform
    objectsShader->setMat4("perspective", mainCamera.getPerspective());
    // Setting camera position for lighting
    objectsShader->setVec3("cameraPos", mainCamera.getPosition());
    // Setting the light matrix
    objectsShader->setMat4("lightSpace", lightSpaceMatrix);
    
    // Render the objects
    for (int i = 0; i < objects.size();i++) {
        GLmethods::render_object(this->objectsShader, &this->objects[i], this->vertexBuffers[i], this->uvBuffers[i],
                                 this->depthMapBuffer, this->depthMapTexture);
    }

    // Render the skybox
    this->mainSkyBox->renderSkyBox(mainCamera.getPerspective(), mainCamera.getProjection());
    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
    glDeleteFramebuffers(1, &this->depthMapBuffer);
    GLmethods::delete_arrays(this->vertexArrays);
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
    delete this->mainSkyBox;
}