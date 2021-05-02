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

#include "ship.cpp"

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

}

// Start the application, ending with call to loop()
void application::start() {
    this->running = true;
    // Correcting main camera to the aspect ratio of the window
    mainCamera.setAspect(float(window_width)/float(window_height));

    // Creating shaders for objects and skyCube (uses files in /shaders folder)
    objectsShader = new shader("./shaders/lightVertex.shader","./shaders/lightFragment.shader");
    // SkyBox //
    this->mainSkyBox = new skyBox("./shaders/skyCube_vertex.shader", "./shaders/skyCube_fragment.shader", "./resources/Skycube/");

    // Object Stuff (including ship) //
    this->myShip = new ship(objectsShader);
    // Demo spheres to give more reference points to show light effects
    this->objects.clear();
    this->objects.push_back(object("./resources/simpleSphere.obj","./resources/simpleTest.bmp",objectsShader));
    this->objects[0].setPosition(glm::vec3(3,0,2));
    this->objects.push_back(object("./resources/simpleSphere.obj","./resources/simpleTest.bmp",objectsShader));
    this->objects[1].setPosition(glm::vec3(10,1,-1));
    this->objects.push_back(object("./resources/simpleSphere.obj","./resources/simpleTest.bmp",objectsShader));
    this->objects[2].setPosition(glm::vec3(-16,-1,-3));

    // Light stuff
    this->lightPos.clear();
    this->lightColors.clear();
    this->lightIntensities.clear();
        // Represented light coming from the Earth
    this->lightPos.push_back(glm::vec4(-200,0,0,1));
    this->lightColors.push_back(glm::vec3(0.8,0.8,1));
    this->lightIntensities.push_back(200);
    this->lightDiffuse.push_back(0.55);
    this->lightAmbient.push_back(0.01);
    this->lightSpecular.push_back(0.00);
    this->lightAlpha.push_back(200);
        // Represented light coming from the Moon
    this->lightPos.push_back(glm::vec4(200,100,-100,1));
    this->lightColors.push_back(glm::vec3(1,1,1));
    this->lightIntensities.push_back(50);
    this->lightDiffuse.push_back(0.55);
    this->lightAmbient.push_back(0.00);
    this->lightSpecular.push_back(0.0);
    this->lightAlpha.push_back(200);
        // A random light to show the dynamic nature of it and illuminate around the ship
    this->lightPos.push_back(glm::vec4(0,0,0,1));
    this->lightColors.push_back(glm::vec3(1,1,1));
    this->lightIntensities.push_back(5);
    this->lightDiffuse.push_back(0.55);
    this->lightAmbient.push_back(0.00);
    this->lightSpecular.push_back(0.01);
    this->lightAlpha.push_back(200);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // Using the one object shader program created in setup, identifying with the id value in the app's struct
    glUseProgram(objectsShader->shaderID);
    // Setting camera transform
    objectsShader->setMat4("camera", mainCamera.getView());
    // Setting perspective transform
    objectsShader->setMat4("perspective", mainCamera.getPerspective());
    // Setting camera position for lighting
    objectsShader->setVec3("cameraPos", mainCamera.getPosition());

    if (lightMove) {
        updateTorpedoLights(ctime - ltime);
        lightPos[2] = glm::vec4(0,10*sin(ctime), 10*cos(ctime),1);
    }

    int numLights = lightPos.size();
    if (numLights > 0) {
        objectsShader->setInt("LightCount", numLights);
        objectsShader->setNvec4("Lpos", numLights, lightPos[0]);
        objectsShader->setNvec3("Lcolor", numLights, lightColors[0]);
        objectsShader->setNfloat("Linten",numLights, lightIntensities[0]);
        objectsShader->setNfloat("Kambient", numLights, lightAmbient[0]);
        objectsShader->setNfloat("Kspecular",numLights, lightSpecular[0]);
        objectsShader->setNfloat("Kdiffuse", numLights, lightDiffuse[0]);;
        objectsShader->setNfloat("U_alpha", numLights, lightAlpha[0]);
    }
    // Render the ship
    this->myShip->renderShip(objectsShader, ctime, ltime);

    // Render other objects
    for (int i = 0; i < objects.size();i++) {
        objects[i].renderObject(objectsShader);
    }

    // Render the skybox
    this->mainSkyBox->renderSkyBox(mainCamera.getPerspective(), mainCamera.getProjection());
    // Swapping buffers to update display
    SDL_GL_SwapWindow(window);
}

// Handle closing the application
void application::close() {
    this->running = false;
    delete this->myShip;
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

// Method for creating torpedo light to emulate "firing"
void application::fireTorpedo() {
    // Setting the initial position of the torpedo
    glm::vec4 start = glm::vec4(myShip->getPos(),1);
    start.x = start.x + 1.6;
    start.y = start.y + 0.4;
    // Creating light by pushing onto vectors
    this->lightPos.push_back(start);
    this->lightColors.push_back(glm::vec3(1,0,0));
    this->lightIntensities.push_back(1);
    this->lightDiffuse.push_back(0.25);
    this->lightAmbient.push_back(0.00);
    this->lightSpecular.push_back(0.1);
    this->lightAlpha.push_back(200);
}

// Method for torpedo light movement, would want to later restructure things to enable this to be encapsulated by more abstract representations
void application::updateTorpedoLights(double delta) {
    // Remove lights that exceed a distance (far enough away that they are considered gone)
    for (int i = lightPos.size()-1; i > 3; i--) {
        if (this->lightPos[i].x > 100.0f || this->lightPos[i].y > 100.0f || this->lightPos[i].z > 100.0f) {
            this->lightPos.erase(this->lightPos.begin() + i);
            this->lightColors.erase(this->lightColors.begin() + i);
            this->lightIntensities.erase(this->lightIntensities.begin() + i);
            this->lightDiffuse.erase(this->lightDiffuse.begin() + i);
            this->lightAmbient.erase(this->lightAmbient.begin() + i);
            this->lightSpecular.erase(this->lightSpecular.begin() + i);
            this->lightAlpha.erase(this->lightAlpha.begin() + i);
        }
    }
    // Speed of the torpedos
    double speed = 2*delta;
    // Update light positions
    for (int i = 3; i < lightPos.size(); i++) {
        // Getting current position, and then moving the light forward along the x axis
        glm::vec4 curr = this->lightPos[i];
        this->lightPos[i] = curr + glm::vec4(speed, 0,0,0);
    }
}
