#ifndef YPRCAMERA_H
#define YPRCAMERA_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/ProgramDefines.h"

/**
\file YPRCamera.h

\brief Header file for YPRCamera.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class YPRCamera

\brief Creates a Yaw-Pitch-Roll camera.

*/

class YPRCamera
{
private:
    glm::vec3 Position; ///< Camera position.
    glm::vec3 View;     ///< Direction of the line of sight.
    glm::vec3 Up;       ///< Direction of up.

public:
    YPRCamera();
    void PositionCamera(float X, float Y, float Z,
                        float vX, float vY, float vZ,
                        float upX, float upY, float upZ);
    void PositionCamera(glm::vec3 pos, glm::vec3 view, glm::vec3 up);

    void setPosition(float X, float Y, float Z);
    void setPosition(glm::vec3 pos);
    void setView(float X, float Y, float Z);
    void setView(glm::vec3 view);
    void setUp(float X, float Y, float Z);
    void setUp(glm::vec3 up);
    void setUpViaY();

    glm::vec3 getPosition();
    glm::vec3 getView();
    glm::vec3 getUp();
    glm::mat4 lookAt();

    void addPitch(float num);
    void addYaw(float num);
    void addRoll(float num);

    void moveForward(float num);
    void moveRight(float num);
    void moveUp(float num);
};

#endif
