#include "SphericalCamera.h"

/**
\file SphericalCamera.cpp

\brief Implementation file for the SphericalCamera class.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor

Position is at (1, 0, 0) pointing toward the origin.

*/

SphericalCamera::SphericalCamera()
{
    r = 1;
    theta = 0;
    psi = 0;
}

/**
\brief Sets the position of the camera

\param R --- Radius of camera to origin.

\param Theta --- Theta rotation on the xz plane counter clockwise from positive x axis.

\param Psi --- Psi rotation from the xz plane to radial.

*/

void SphericalCamera::setPosition(float R, float Theta, float Psi)
{
    r = R;
    theta = Theta;
    psi = Psi;
}

/**
\brief Returns the lookat matrix for the current position and line of sight to the origin.

\return The lookat matrix for the current position and line of sight to the origin.

*/

glm::mat4 SphericalCamera::lookAt()
{
    glm::vec3 eye = glm::vec3(r*cos(psi*deg)*cos(theta*deg),r*sin(psi*deg),r*cos(psi*deg)*sin(theta*deg));
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);

    return glm::lookAt(eye, center, up);
}


glm::vec3 SphericalCamera::getPosition()
{
    return glm::vec3(r*cos(psi*deg)*cos(theta*deg),r*sin(psi*deg),r*cos(psi*deg)*sin(theta*deg));
}

/**
\brief Adds to the camera radius.

\param num --- amount to add to the radius of the camera.

*/

void SphericalCamera::addR(float num)
{
    r += num;
    if (r < 0.000001f) r = 0.000001f;
}

/**
\brief Adds to the theta angle of the camera.

\param num --- amount to add to the theta angle of the camera.

*/

void SphericalCamera::addTheta(float num)
{
    theta += num;
    if (theta > 360) theta -= 360;
    if (theta < 0) theta += 360;
}

/**
\brief Adds to the psi angle of the camera.

\param num --- amount to add to the psi angle of the camera.

*/

void SphericalCamera::addPsi(float num)
{
    psi += num;
    if (psi > 90) psi = 90;
    if (psi < -90) psi = -90;
}

/**
\brief Sets the radius of the camera.

\param num --- new value of the radius.

*/

void SphericalCamera::setR(float num)
{
    r = num;
    if (r < 0.000001f) r = 0.000001f;
}

/**
\brief Sets the theta angle of the camera.

\param num --- New value of the theta angle.

*/

void SphericalCamera::setTheta(float num)
{
    theta = num;
    while (theta > 360) theta -= 360;
    while (theta < 0) theta += 360;
}

/**
\brief Sets the psi angle of the camera.

\param num --- New value of the psi angle.

*/

void SphericalCamera::setPsi(float num)
{
    psi = num;
    if (psi > 90) psi = 90;
    if (psi < -90) psi = -90;
}

/**
\brief Returns the current value of the camera radius.

\return The current value of the camera radius.

*/

float SphericalCamera::getR()
{
    return r;
}

/**
\brief Returns the current value of the theta angle of the camera.

\return The current value of the theta angle of the camera.

*/

float SphericalCamera::getTheta()
{
    return theta;
}

/**
\brief Returns the current value of the psi angle of the camera.

\return The current value of the psi angle of the camera.

*/

float SphericalCamera::getPsi()
{
    return psi;
}
