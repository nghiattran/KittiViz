#include "YPRCamera.h"

/**
\file YPRCamera.cpp

\brief Implementation file for the YPRCamera class.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor

Position is at the origin, the view vector is pointing down the negative z-axis,
up is the positive y direction.

*/

YPRCamera::YPRCamera()
{
    Position.x = 0;
    Position.y = 0;
    Position.z = 0;
    View.x = 0;
    View.y = 0;
    View.z = -1;
    Up.x = 0;
    Up.y = 1;
    Up.z = 0;
}

/**
\brief Sets the position, view and up for the camera.

The method will also normalize the view and up vectors.

\param X --- X coordinate to the camera position.

\param Y --- Y coordinate to the camera position.

\param Z --- Z coordinate to the camera position.

\param vX --- X coordinate to the viewing vector.

\param vY --- Y coordinate to the viewing vector.

\param vZ --- Z coordinate to the viewing vector.

\param upX --- X coordinate to the up vector.

\param upY --- Y coordinate to the up vector.

\param upZ --- Z coordinate to the up vector.

*/

void YPRCamera::PositionCamera(float X, float Y, float Z, float vX, float vY, float vZ, float upX, float upY, float upZ)
{
    Position.x = X;
    Position.y = Y;
    Position.z = Z;
    View.x = vX;
    View.y = vY;
    View.z = vZ;
    Up.x = upX;
    Up.y = upY;
    Up.z = upZ;

    Up = glm::normalize(Up);
    View = glm::normalize(View);
}

/**
\brief Sets the position, view and up for the camera.

The method will also normalize the view and up vectors.

\param pos --- Position vector.

\param view --- View vector.

\param up --- Up vector.

*/

void YPRCamera::PositionCamera(glm::vec3 pos, glm::vec3 view, glm::vec3 up)
{
    Position = pos;
    View = view;
    Up = up;

    Up = glm::normalize(Up);
    View = glm::normalize(View);
}

/**
\brief Sets the position for the camera.

\param X --- X coordinate to the camera position.

\param Y --- Y coordinate to the camera position.

\param Z --- Z coordinate to the camera position.

*/

void YPRCamera::setPosition(float X, float Y, float Z)
{
    Position.x = X;
    Position.y = Y;
    Position.z = Z;
}

/**
\brief Sets the position for the camera.

\param pos --- Position vector.

*/

void YPRCamera::setPosition(glm::vec3 pos)
{
    Position = pos;
}

/**
\brief Sets the view vector for the camera.

The method will also normalize the view vector.

\param X --- X coordinate to the viewing vector.

\param Y --- Y coordinate to the viewing vector.

\param Z --- Z coordinate to the viewing vector.

*/

void YPRCamera::setView(float X, float Y, float Z)
{
    View.x = X;
    View.y = Y;
    View.z = Z;
    View = glm::normalize(View);
}

/**
\brief Sets the view vector for the camera.

The method will also normalize the view vector.

\param view --- View vector.

*/

void YPRCamera::setView(glm::vec3 view)
{
    View = view;
    View = glm::normalize(View);
}

/**
\brief Sets the up vector for the camera.

The method will also normalize the up vector.

\param X --- X coordinate to the up vector.

\param Y --- Y coordinate to the up vector.

\param Z --- Z coordinate to the up vector.

*/

void YPRCamera::setUp(float X, float Y, float Z)
{
    Up.x = X;
    Up.y = Y;
    Up.z = Z;
    Up = glm::normalize(Up);
}

/**
\brief Sets the up vector for the camera.

The method will also normalize the up vector.

\param up --- Up vector.

*/

void YPRCamera::setUp(glm::vec3 up)
{
    Up = up;
    Up = glm::normalize(Up);
}

/**
\brief Sets the up vector for the camera in the positive y direction.

This method will also normalize the up  vector and keep it orthogonal to
the viewing vector.

*/

void YPRCamera::setUpViaY()
{
    glm::vec3 Y(0,1,0);
    glm::vec3 V = View;
    glm::vec3 P = glm::dot(Y, V)/(glm::length(V)*glm::length(V))*V;
    Up = glm::normalize(Y-P);
}

/**
\brief Returns the current position of the camera.

\return The current position of the camera.

*/

glm::vec3 YPRCamera::getPosition()
{
    return Position;
}

/**
\brief Returns the current viewing vector of the camera.

\return The current viewing vector of the camera.

*/

glm::vec3 YPRCamera::getView()
{
    return View;
}

/**
\brief Returns the current up vector of the camera.

\return The current up vector of the camera.

*/

glm::vec3 YPRCamera::getUp()
{
    return Up;
}

/**
\brief Adds to the current pitch of the camera.

\param num --- The degree measurement of the amount to add to the pitch of the camera.

*/

void YPRCamera::addPitch(float num)
{
    // Calculate the right vector.
    glm::vec3 mn = glm::cross(View, Up);

    // Create the rotation matrix for rotation around the right vector by an angle of num.
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), num*degf, mn);

    // Convert the view and up vectors to 4D.
    glm::vec4 View4 = glm::vec4(View, 1);
    glm::vec4 Up4 = glm::vec4(Up, 1);

    // Rotate the view and up vectors.
    View4 = rot*View4;
    Up4 = rot*Up4;

    // Convert the transformed view and up vectors to 3D.
    Up = Up4.xyz();
    View = View4.xyz();

    // Normalize the results.
    Up = glm::normalize(Up);
    View = glm::normalize(View);
}

/**
\brief Adds to the current yaw of the camera.

\param num --- The degree measurement of the amount to add to the yaw of the camera.

*/

void YPRCamera::addYaw(float num)
{
    // Create the rotation matrix for rotation around the up vector by an angle of num.
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), num*degf, Up);

    // Convert the view vector to 4D.
    glm::vec4 View4 = glm::vec4(View, 1);

    // Rotate the view vector.
    View4 = rot*View4;

    // Convert the transformed view vector to 3D.
    View = View4.xyz();

    // Normalize the result.
    View = glm::normalize(View);
}

/**
\brief Adds to the current roll of the camera.

\param num --- The degree measurement of the amount to add to the roll of the camera.

*/

void YPRCamera::addRoll(float num)
{
    // Create the rotation matrix for rotation around the view vector by an angle of num.
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), num*degf, View);

    // Convert the up vector to 4D.
    glm::vec4 Up4 = glm::vec4(Up, 1);

    // Rotate the up vector.
    Up4 = rot*Up4;

    // Convert the transformed up vector to 3D.
    Up = Up4.xyz();

    // Normalize the result.
    Up = glm::normalize(Up);
}

/**
\brief Returns the lookat matrix for the current position, line of sight, and up
vectors.

\return The lookat matrix for the current position, line of sight, and up vectors.

*/

glm::mat4 YPRCamera::lookAt()
{
    return glm::lookAt(Position, Position + View, Up);
}

/**
\brief Moves the camera forward.

\param num --- Amount to move the camera forward.

*/

void YPRCamera::moveForward(float num)
{
    float mod = glm::length(View);
    if (mod < 0.000001f) return;

    Position += View/mod*num;
}

/**
\brief Moves the camera right.

\param num --- Amount to move the camera to the right.

*/

void YPRCamera::moveRight(float num)
{
    glm::vec3 mn = glm::cross(View, Up);

    float mod = glm::length(mn);
    if (mod < 0.000001f) return;

    Position += mn/mod*num;
}

/**
\brief Moves the camera up.

\param num --- Amount to move the camera up.

*/

void YPRCamera::moveUp(float num)
{
    float mod = glm::length(Up);
    if (mod < 0.000001f) return;

    Position += Up/mod*num;
}
