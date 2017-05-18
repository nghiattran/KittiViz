#ifndef AXES_H_INCLUDED
#define AXES_H_INCLUDED

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(x) ((const void*) (x))

/**
\file Axes.h
\brief Header file for Axes.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class Axes

\brief Axes for development, allows the programmer to see the positioning of objects
easier.

Draws the xyz axes on the screen, each axis has the coordinate range of [-1, 1]. The
x-axis is in red, with the positive x-axis is bright red and the negative x-axis in
a darker red.  The same is true for the y-axis in green and the z-axis in blue.

*/

class Axes {
private:
    GLuint vboptr;  ///< ID for the VBO.
    GLuint bufptr;  ///< ID for the array buffer.
    GLuint eboptr;  ///< ID for the index array buffer.

    void LoadDataToGraphicsCard();

public:
    Axes();
    ~Axes();

    void draw();
};

#endif // AXES_H_INCLUDED
