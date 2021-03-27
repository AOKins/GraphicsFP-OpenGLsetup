#include "../headers/app.h"

// Handler for general events
// Input: event - pointer to SDL_Event type
//        deltaTime - time since last frame render, useful if input relies on relative to last frame
// Output: identifies a relevant event and calls appriopriate metehod
void application::event(SDL_Event * event, double deltaTime) {
    // Determine the event and call appriopriate method
    switch (event->type) {
        case (SDL_QUIT):
            close();
            break;
        case (SDL_KEYDOWN):
            onKeyPress(&event->key, deltaTime);
            break;
    }
}

// Handler for keyboard presses - https://wiki.libsdl.org/SDL_Scancode was useful in identifying specific keys wanted
// Input: key_event - SDL keyboard event object
// Output: Depending on recognized key press, changes the mode for transformation behavior the application will do in render()
void application::onKeyPress(SDL_KeyboardEvent * key_event, double deltaTime) {
    switch(key_event->keysym.scancode) {
        case (SDL_SCANCODE_ESCAPE):
            this->close();
            break;
        // Function keys change polygon mode between filled and lines (wireframe)
        case (SDL_SCANCODE_F1):
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case (SDL_SCANCODE_F2):
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(3.0f);
            break;
        // Adjust FOV
        case (SDL_SCANCODE_F5):
            mainCamera.setFOV(mainCamera.getFOV()-deltaTime);
            break;
        case (SDL_SCANCODE_F6):
            mainCamera.setFOV(mainCamera.getFOV()+deltaTime);
            break;
        // Movement inputs
        case (SDL_SCANCODE_W):
            mainCamera.updatePos(-deltaTime, Z);
            break;
        case (SDL_SCANCODE_S):
            mainCamera.updatePos(deltaTime, Z);
            break;
        case (SDL_SCANCODE_A):
            mainCamera.updatePos(-deltaTime, X);
            break;
        case (SDL_SCANCODE_D):
            mainCamera.updatePos(deltaTime, X);
            break;
        case (SDL_SCANCODE_Z):
            mainCamera.updatePos(deltaTime, Y);
            break;
        case (SDL_SCANCODE_X):
            mainCamera.updatePos(-deltaTime, Y);
            break;
        // Simple camera rotation controls
        case (SDL_SCANCODE_LEFT):
            mainCamera.turnLeftRight(-deltaTime);
            break;
        case (SDL_SCANCODE_RIGHT):
            mainCamera.turnLeftRight(deltaTime);
            break;
        case (SDL_SCANCODE_UP):
            mainCamera.turnUpDown(deltaTime);
            break;
        case (SDL_SCANCODE_DOWN):
            mainCamera.turnUpDown(-deltaTime);
            break;
        default: // If unrecognized, default to doing nothing
            break;
    }
}
