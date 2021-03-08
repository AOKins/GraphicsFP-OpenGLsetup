#ifndef _MYAPP_H_
#define _MYAPP_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "shader.h"
#include <math.h>
#include <GLM/glm.hpp>

class application {
protected: 
    bool running;
    SDL_Window * window;
    SDL_GLContext context;
    
    GLuint vertexArrayID, vertexBufferID, elementBufferID;
    
    shader * shaderApp;
        
    int window_width;
    int window_height;

    int mouseClick = 0;

    // Creating an array of 3D positions for each point of a simple triangle
    glm::vec3 triangleVertices[3] = {
        // Original Triangle data
        glm::vec3(0.5f,  0.5f, 0.0f),
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f)
    };

    // Array for background color, default value is solid white
    GLfloat bg_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};


public:
    // Default constructor, calls initialize and sets running to false
    application();

    // Sets up the window and context, also initialization of GLEW
    void initialize();

    void start();

    void loop();
    
    void render(double ctime, double ltime);

    void event(SDL_Event * event);

    void close();

    bool isRunning();

    void onMouseMove(SDL_MouseMotionEvent * mouse_event);

    void onMouseButton(SDL_MouseButtonEvent * button);
};

#endif