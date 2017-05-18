#ifndef SPHERICALCAMERA_H
#define SPHERICALCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#include "../utils/ProgramDefines.h"

/**
\file SphericalCamera.h

\brief Header file for SphericalCamera.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class SphericalCamera

\brief Creates a spherical camera, that is, one that sits on a sphere of radius r and always
points to the origin.

*/

class SphericalCamera
{
    private:
        float r;      ///< Radius of camera to origin.
        float theta;  ///< Theta rotation on the xz plane counter clockwise from positive x axis.
        float psi;    ///< Psi rotation from the xz plane to radial.

    public:
        SphericalCamera();
        void setPosition(float R, float Theta, float Psi);
        glm::mat4 lookAt();

        glm::vec3 getPosition();

        void addR(float num);
        void addTheta(float num);
        void addPsi(float num);

        void setR(float num);
        void setTheta(float num);
        void setPsi(float num);

        float getR();
        float getTheta();
        float getPsi();
};

#endif
