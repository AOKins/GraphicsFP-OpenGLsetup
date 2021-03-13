#include <glm/glm.hpp>
#include "../headers/app.h"

glm::mat4x4 application::getTranslate(double ctime) {
    switch (transformMode) {
        case (TRANSLATION):
            return glm::mat4x4(
                1.0f, 0.0f, 0.0f, this->pos_x,
                0.0f, 1.0f, 0.0f, this->pos_y,
                0.0f, 0.0f, 1.0f, this->pos_z,
                0.0f, 0.0f, 0.0f,       1.0f);
            break;
        case (FUN):
            return glm::mat4x4( // Applying translation AFTER rotation so that it's a rotating cube moving in a clear circular motion and this time also adding Z translation
                1.0f, 0.0f, 0.0f, glm::cos(ctime)/2.0f,
                0.0f, 1.0f, 0.0f, glm::sin(ctime)/2.0f,
                0.0f, 0.0f, 1.0f, 0.1*glm::sin(ctime/1.0f),
                0.0f, 0.0f, 0.0f,                 1.0f);
        default: // Don't do anything
            return glm::mat4x4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
            break;
    }
}

glm::mat4x4 application::getScale(double ctime) { // Simply scaling the cube by 2
        switch (transformMode) {
            case(SHEARING):
                return glm::mat4x4( // Setting shearing demo to be dependent on time to show dynamic shearing effect that is "wobbly"
                    0.5f, glm::cos(ctime)/2, 0.0f, 0.0f,
                    glm::cos(ctime)/2, 0.5f, 0.0f, 0.0f,
                    0.0f,            0.0f, 1.0f, 0.0f,
                    0.0f,            0.0f, 0.0f, 1.0f);
                break;
            case (ROTATION_X):
                return glm::mat4x4( // Scaling it down to half its original size
                0.75f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.75f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.75f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
                break;
            case (ROTATION_Y):
                return glm::mat4x4( // Scaling it down to half its original size
                0.75f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.75f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.75f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
                break;
            case (ROTATION_Z):
                return glm::mat4x4( // Scaling it down to half its original size
                0.75f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.75f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.75f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
                break;

            case (FUN):
                return glm::mat4x4( // Scaling it down to half its original size
                0.5f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.5f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
                break;
            default:
                return glm::mat4x4(
                    scaleSize,0.0f,0.0f,0.0f,
                    0.0f,scaleSize,0.0f,0.0f,
                    0.0f,0.0f,scaleSize,0.0f,
                    0.0f,0.0f,     0.0f,1.0f);
        }}

glm::mat4x4 application::getRotation(double ctime) {
        switch (transformMode) {
            case (SHEARING):
                return glm::mat4x4( // Rotation about the y axis to show the shear effect in 3D
                    glm::cos(ctime/4.0f),   0.0f,  -glm::sin(ctime/4.0f),   0.0f,
                    0.0f,                   1.0f,                   0.0f,   0.0f,
                    glm::sin(ctime/4.0f),   0.0f,   glm::cos(ctime/4.0f),   0.0f,
                    0.0f,                   0.0f,                   0.0f,   1.0f);
                break;
            case (ROTATION_Z):
                return glm::mat4x4( // Rotation about the z axis
                     glm::cos(ctime/2.0f), glm::sin(ctime/2.0f), 0.0f, 0.0f,
                    -glm::sin(ctime/2.0f), glm::cos(ctime/2.0f), 0.0f, 0.0f,
                                     0.0f,                 0.0f, 1.0f, 0.0f,
                                     0.0f,                 0.0f, 0.0f, 1.0f);
                break;
            case (ROTATION_Y):
                return glm::mat4x4( // Rotation about the y axis
                    glm::cos(ctime/2.0f),   0.0f,  -glm::sin(ctime/2.0f),   0.0f,
                    0.0f,                   1.0f,                   0.0f,   0.0f,
                    glm::sin(ctime/2.0f),   0.0f,   glm::cos(ctime/2.0f),   0.0f,
                    0.0f,                   0.0f,                   0.0f,   1.0f);
                break;
            case (ROTATION_X):
                return glm::mat4x4( // Rotation about the x axis
                    1.0f,                   0.0f,                 0.0f, 0.0f,
                    0.0f,   glm::cos(ctime/2.0f), glm::sin(ctime/2.0f), 0.0f,
                    0.0f,  -glm::sin(ctime/2.0f), glm::cos(ctime/2.0f), 0.0f,
                    0.0f,                   0.0f,                 0.0f, 1.0f);
                break;
            case (FUN):
            return glm::mat4x4(  // Rotation on x plane
                1.0f,                   0.0f,                 0.0f, 0.0f,
                0.0f,   glm::cos(ctime/2.1f), glm::sin(ctime/2.1f), 0.0f,
                0.0f,  -glm::sin(ctime/2.1f), glm::cos(ctime/2.1f),  0.0f,
                0.0f,                   0.0f,                  0.0f, 1.0f)
                * glm::mat4x4(  // Rotation on y plane
                glm::cos(ctime/2.0f),   0.0f, -glm::sin(ctime/2.0f), 0.0f,
                0.0f,                   1.0f,                  0.0f, 0.0f,
                glm::sin(ctime/2.0f),   0.0f,  glm::cos(ctime/2.0f), 0.0f,
                0.0f,                   0.0f,                  0.0f, 1.0f);
                break;
            default:
                return glm::mat4x4(
                    1.0f,0.0f,0.0f,0.0f,
                    0.0f,1.0f,0.0f,0.0f,
                    0.0f,0.0f,1.0f,0.0f,
                    0.0f,0.0f,0.0f,1.0f
                );
        }
}

glm::mat4x4 application::getProjection(double ctime) {
    return glm::mat4x4( // Performing a parallel projection along the y=0 plane (recommended to use wireframe mode to be able to see it by pressing F2)
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

glm::mat4x4 application::getPerspective(double ctime) {
        switch (transformMode) {
            case (FUN):
                glm::mat4x4( // Perform the projection with z = 2 so we got some depth going on
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.5f, 1.0f);
                break;
        }
        return glm::mat4x4( // projection with plane at z = 2
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.5f, 1.0f);
}

glm::mat4x4 application::getInverse(glm::mat4x4 orig_scale, glm::mat4x4 orig_rotate, glm::mat4x4 orig_transl) {
    switch(transformMode) {
        case(TRANSLATION): // In translation mode, invert the translation
            return glm::inverse(orig_transl);
            break;
        case(FUN): // In fun mode, inv
            return glm::inverse(orig_rotate);
            break;
        case(ROTATION_X): // In fun mode, inv
            return glm::inverse(orig_rotate);
            break;
        case(ROTATION_Y): // In fun mode, inv
            return glm::inverse(orig_rotate);
            break;
        case(ROTATION_Z): // In fun mode, inv
            return glm::inverse(orig_rotate);
            break;
    }
    return glm::mat4x4(1.0f,0.0f,0.0f,0.0f,
                        0.0f,1.0f,0.0f,0.0f,
                        0.0f,0.0f,1.0f,0.0f,
                        0.0f,0.0f,0.0f,1.0f);
}