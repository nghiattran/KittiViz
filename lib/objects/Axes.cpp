#include "Axes.h"

/**
\file Axes.cpp
\brief Implementation for graphing the xyz axes.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor

Calls the data loading method to load the axes data to the graphics card.

*/

Axes::Axes()
{
    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Removes allocated data from the graphics card.

*/

Axes::~Axes()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}

/**
\brief Loads the vertex and color data to the graphics card.

*/

void Axes::LoadDataToGraphicsCard()
{
    GLint vPosition = 0;
    GLint vColor = 1;

    GLushort indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    GLfloat points[] = {0, 0, 0, 1,
                        1, 0, 0, 1,
                        0, 0, 0, 1,
                        -1, 0, 0, 1,
                        0, 0, 0, 1,
                        0, 1, 0, 1,
                        0, 0, 0, 1,
                        0, -1, 0, 1,
                        0, 0, 0, 1,
                        0, 0, 1, 1,
                        0, 0, 0, 1,
                        0, 0, -1, 1,
                       };

    GLfloat colors[] = {1, 0, 0,
                        1, 0, 0,
                        0.25, 0, 0,
                        0.25, 0, 0,
                        0, 1, 0,
                        0, 1, 0,
                        0, 0.25, 0,
                        0, 0.25, 0,
                        0, 0, 1,
                        0, 0, 1,
                        0, 0, 0.25,
                        0, 0, 0.25
                       };

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glGenBuffers(1, &eboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &bufptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

/**
\brief Draws the axes to the screen.

*/

void Axes::draw()
{
    glLineWidth(2);
    glBindVertexArray(vboptr);
    glDrawElements(GL_LINES, 12, GL_UNSIGNED_SHORT, NULL);
    glLineWidth(1);
}
