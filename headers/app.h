#ifndef _MYAPP_H_
#define _MYAPP_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>

#include "object.h"
#include "shader.h"
#include "camera.h"
#include "skyBox.h"

#include "helicopter.h"

class application {
protected: 
    bool running;

    SDL_Window * window;
    SDL_GLContext context;
    shader * objectsShader, *shadowMapShader;

    // Dimensions of the window
    int window_width, window_height;
    // Tracker for number of mouse clicks made (left increases, right decreases)
    int mouseClick = 0;

    float fill_factor;

    // Vector that holds object data
    std::vector<object> objects;

    // Helicopter object
    helicopter * myHelicopter;

    // Vectors for ID values
    std::vector<GLuint> elementBuffers;
    std::vector<GLuint> vertexArrays;
    std::vector<GLuint> vertexBuffers;
    std::vector<GLuint> normalArrays;
    std::vector<GLuint> normalBuffers;
    std::vector<GLuint> uvBuffers;
    std::vector<GLuint> textureIDs;
        // Shadow mapping values
    GLuint depthMapTexture, shadowRes, depthMapBuffer;
    
    // Vectors for light info
    std::vector<glm::vec3> lightPos;
    glm::vec3 sky_light_pos; // Position of the skylight relative to the camera
    std::vector<glm::vec3> lightColors;
    std::vector<float> lightIntensities;
    std::vector<float> lightAmbient;
    std::vector<float> lightDiffuse;
    std::vector<float> lightSpecular;
    std::vector<float> lightAlpha;

    // Simple boolean, if false then set the light to not move
    bool renderMap = false;

    // Camera that user controls
    camera mainCamera;

    // SkyBox container
    skyBox * mainSkyBox;
public:
    // Default constructor, calls initialize and sets running to false
    application();
    ~application();

    // Sets up the window and context, also initialization of GLEW
    void initialize();

    // Handles the start of the application
    void start();

    // Simple loop method that keeps the application running with processing of events and calls render
    void loop();
    
    // Perform the necessarry rendering
    void render(double ctime, double ltime);

    // Processor for an event, identifies the appriopriate handler for an identified event
    void event(SDL_Event * event, double deltaTime);

    // Simple getter for getting running boolean value
    bool isRunning();

    // Handler for when the mouse moves
    void onMouseMove(SDL_MouseMotionEvent * mouse_event);

    // Handler for when a mouse button is pressed
    void onMouseButton(SDL_MouseButtonEvent * button);

    // Handler for when a key press is made
    void onKeyPress(SDL_KeyboardEvent * key_event, double deltaTime);    
    
    void continuousKeyInput(double deltaTime);
    
    void close();
};

#endif