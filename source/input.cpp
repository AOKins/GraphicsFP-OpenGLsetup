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
            mainCamera.setFOV(mainCamera.getFOV()-deltaTime*2.0f);
            break;
        case (SDL_SCANCODE_F6):
            mainCamera.setFOV(mainCamera.getFOV()+deltaTime*2.0f);
            break;
        case (SDL_SCANCODE_O):
            myShip->changeStatus();
            break;
        default: // If unrecognized, default to doing nothing
            break;
    }
}

// Handler for keyboard presses that are discrete presses (like WASD movement)
// Input: checks status for various keys
// Output: responds accordingly
void application::continuousKeyInput(double deltaTime) {
    const Uint8 * keyboardState = SDL_GetKeyboardState(NULL);
    // Movement inputs
    if (keyboardState[SDL_SCANCODE_W] == SDL_PRESSED) {
        mainCamera.moveFoward(deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_S] == SDL_PRESSED) {
        mainCamera.moveFoward(-deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_A] == SDL_PRESSED) {
        mainCamera.moveLeft(deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_D] == SDL_PRESSED) {
        mainCamera.moveLeft(-deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_SPACE] == SDL_PRESSED) {
        mainCamera.moveUp(deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_LCTRL] == SDL_PRESSED) {
        mainCamera.moveUp(-deltaTime*2.0f);
    }
    // Camera look-around keyboard controls
    if (keyboardState[SDL_SCANCODE_RIGHT] == SDL_PRESSED) {
        mainCamera.turnRight(deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_LEFT] == SDL_PRESSED) {
        mainCamera.turnRight(-deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_UP] == SDL_PRESSED) {
        mainCamera.turnUp(deltaTime*2.0f);
    }
    if (keyboardState[SDL_SCANCODE_DOWN] == SDL_PRESSED) {
        mainCamera.turnUp(-deltaTime*2.0f);
    }
}
