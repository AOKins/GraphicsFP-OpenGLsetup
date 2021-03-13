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
    // Enumerated values for the modes that the application will operate under
    enum MODE {
        NONE, // Don't do anything but render the current state
        TRANSLATION,
        ROTATION_X, // Simply do some rotation about the stated axis
        ROTATION_Y, // Simply do some rotation about the stated axis
        ROTATION_Z, // Simply do some rotation about the stated axis
        SHEARING, // Demonstrate shearing
        FUN // Something different that uses multiple transformations
    };
protected: 
    bool running;
    // Some transforms or operations are better described through toggled additional renders
    bool show_inverse, show_projection, show_reflection;
    // How big to scale the original cube
    float scaleSize;

    SDL_Window * window;
    SDL_GLContext context;    
    shader * shaderApp;

    MODE transformMode; // Current mode of transformations the application is running
    
    GLuint vertexArrayID, vertexBufferID, elementBufferID;
    // Dimensions of the window
    int window_width, window_height;
    // Tracker for number of mouse clicks made (left increases, right decreases)
    int mouseClick = 0;

    // Cube data //
    // Number of triangles expected to render
    int num_triangles;
    float pos_x, pos_y, pos_z; // Coordinates of the cube object in world space
    vertexColor * triangleData;    // Creating an array of 3D positions for each point of a triangle

    // Background color
    GLfloat bg_color[4] = {0.0f,0.0f,0.0f,1.0f};

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
    void event(SDL_Event * event);

    // Simple getter for getting running boolean value
    bool isRunning();

    // Handler for when the mouse moves
    void onMouseMove(SDL_MouseMotionEvent * mouse_event);

    // Handler for when a mouse button is pressed
    void onMouseButton(SDL_MouseButtonEvent * button);

    // Handler for when a key press is made
    void onKeyPress(SDL_KeyboardEvent * key_event);    
    
    void close();

    // Methods for handling generating the appriopriately desired matrix depending on ctime and mode
    // Scaling matrix
    glm::mat4x4 getScale(double ctime);
    // Translation of the object which depends on the coordinates of the cube
    glm::mat4x4 getTranslate(double ctime);
    // Rotation of the cube
    glm::mat4x4 getRotation(double ctime);
    // Reflection that also does an equivalent 180 rotation
    glm::mat4x4 getReflection(double ctime);
    // Project onto a flat plane
    glm::mat4x4 getProjection(double ctime);
    // Used all the time, gives a sense of 3D
    glm::mat4x4 getPerspective(double ctime);
    // Generate an appriopriate inverse depending on mode
    glm::mat4x4 getInverse(glm::mat4x4 orig_scale, glm::mat4x4 orig_rotate, glm::mat4x4 orig_transl);
};

#endif