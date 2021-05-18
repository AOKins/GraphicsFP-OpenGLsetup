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
        case (SDL_SCANCODE_P):
            renderMap = !renderMap;
            break;
        case (SDL_SCANCODE_O):
            this->fill_factor = this->fill_factor + 0.1f;
            glPolygonOffset(this->fill_factor,0.1f);
            std::cout << this->fill_factor << std::endl;
            break;
        case (SDL_SCANCODE_I):
            this->fill_factor = this->fill_factor - 0.1f;
            glPolygonOffset(this->fill_factor,0.1f);
            std::cout << this->fill_factor << std::endl;
            break;
        case(SDL_SCANCODE_R):
            this->lightColors[0].r += 0.1;
            break;
        case(SDL_SCANCODE_F):
            this->lightColors[0].r -= 0.1;
            break;
        case(SDL_SCANCODE_T):
            this->lightColors[0].g += 0.1;
            break;
        case(SDL_SCANCODE_G):
            this->lightColors[0].g -= 0.1;
            break;
        case(SDL_SCANCODE_Y):
            this->lightColors[0].b += 0.1;
            break;
        case(SDL_SCANCODE_H):
            this->lightColors[0].b -= 0.1;
            break;
        case(SDL_SCANCODE_KP_PERIOD):
            std::cout << myHelicopter->getLift().y/1000;
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
        mainCamera.moveFoward(deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_S] == SDL_PRESSED) {
        mainCamera.moveFoward(-deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_A] == SDL_PRESSED) {
        mainCamera.moveLeft(deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_D] == SDL_PRESSED) {
        mainCamera.moveLeft(-deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_SPACE] == SDL_PRESSED) {
        mainCamera.moveUp(deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_LCTRL] == SDL_PRESSED) {
        mainCamera.moveUp(-deltaTime*4.0f);
    }
    // Camera look-around keyboard controls
    if (keyboardState[SDL_SCANCODE_RIGHT] == SDL_PRESSED) {
        mainCamera.turnRight(deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_LEFT] == SDL_PRESSED) {
        mainCamera.turnRight(-deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_UP] == SDL_PRESSED) {
        mainCamera.turnUp(deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_DOWN] == SDL_PRESSED) {
        mainCamera.turnUp(-deltaTime*4.0f);
    }
    if (keyboardState[SDL_SCANCODE_KP_DIVIDE] == SDL_PRESSED) {
        this->myHelicopter->increaseThrottle(-deltaTime);
    }
    if (keyboardState[SDL_SCANCODE_KP_MULTIPLY] == SDL_PRESSED) {
        this->myHelicopter->increaseThrottle(deltaTime);
    }
    if (keyboardState[SDL_SCANCODE_KP_8] == SDL_PRESSED) {
        this->myHelicopter->pitchForward(deltaTime);
    }
    if (keyboardState[SDL_SCANCODE_KP_2] == SDL_PRESSED) {
        this->myHelicopter->pitchForward(-deltaTime);
    }
   if (keyboardState[SDL_SCANCODE_KP_4] == SDL_PRESSED) {
        this->myHelicopter->turnLeft(deltaTime);
    }
    if (keyboardState[SDL_SCANCODE_KP_6] == SDL_PRESSED) {
        this->myHelicopter->turnLeft(-deltaTime);
    }

}
