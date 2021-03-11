#ifndef _MYAPP_H_
#define _MYAPP_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include "vertexColor.h"
#include "shader.h"

class application {
private:
    // Enumerated values for the modes that the application will operate under
    enum MODE {
        NONE, // Don't do anything but render the current state
        TRANSLATION,
        SCALE,
        ROTATION_X,
        ROTATION_Y,
        ROTATION_Z,
        REFLECTION,
        SHEARING,
        PROJECTION,
        FUN
    };
protected: 
    bool running;
    SDL_Window * window;
    SDL_GLContext context;

    MODE transformMode; // Current mode of transformations the application is running
    
    GLuint vertexArrayID, vertexBufferID, elementBufferID;
    
    shader * shaderApp;
        
    int window_width;
    int window_height;

    int mouseClick = 0;

    // Creating an array of 3D positions for each point of two simple triangles to create a square
    vertexColor * triangleData;
    int num_triangles;

    // Array for background color, default value is solid white
    GLfloat bg_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};

public:
    // Default constructor, calls initialize and sets running to false
    application();
    ~application();

    // Sets up the window and context, also initialization of GLEW
    void initialize();

    void start();

    void loop();
    
    void render(double ctime, double ltime);

    void event(SDL_Event * event);

    // Simple getter for getting running boolean value
    bool isRunning();

    void onMouseMove(SDL_MouseMotionEvent * mouse_event);

    void onMouseButton(SDL_MouseButtonEvent * button);

    void onKeyPress(SDL_KeyboardEvent * key_event);    
    
    void close();
};

#endif