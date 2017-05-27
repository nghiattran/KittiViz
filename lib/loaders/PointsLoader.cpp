#include "PointsLoader.h"

/**
\file PointsLoader.cpp
\brief Helper class that loads and graph velodyne cloud points.

\author    Nghia Tran
\version   1.0
\date      Written: 5/10/2017

*/

/**
\brief Constructor

Loads the shaders, and sets some initial data settings.

*/

PointsLoader* PointsLoader::mInstance = NULL;

PointsLoader::PointsLoader()
{
    // This creates our identifier and puts it in vbo
    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);
}

/**
\brief Destructor.

*/

PointsLoader::~PointsLoader()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
}

/**
\brief Singleton constructor.

Create a singleton object of PointsLoader.

*/

PointsLoader* PointsLoader::getInstance() {
    if (!mInstance)
    {
        mInstance = new PointsLoader;
    }
    return mInstance;
}

/**
\brief Loads the vertex and color data to the graphics card by vector<float>.

*/

void PointsLoader::update(CloudPoints cp)
{
    isLoaded = true;

    std::vector<float> data = cp.getData();
    
    GLuint vPosition = 0;
    GLuint vColor = 1;

    num_pts = data.size() / 4;
    GLfloat points[num_pts * 4];
    for (uint i = 0; i < num_pts; i++)
    {
        points[i*4] = data[i*4];
        points[i*4 + 1] = -data[i*4 + 1];
        // Z axis need to be shifted down by 2.9 ft because that is the height of velodyne radar
        points[i*4 + 2] = -(data[i*4 + 2]);
        points[i*4 + 3] = 1;
    }


    GLushort indices[num_pts];
    for (uint i = 0; i < num_pts; i++)
    {
        indices[i] = i;
    }

    GLfloat colors[num_pts * 3];
    for (uint i = 0; i < num_pts; i++)
    {
        // float dst = sqrt(pow(points[3*i], 2) + pow(points[3*i+1], 2));
        colors[i * 3] = 0;
        colors[i * 3 + 1] = 1;
        colors[i * 3 + 2] = 0;
    }

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) * 2 + sizeof(colors), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

/**
\brief Load and draw cloud points.

*/

void PointsLoader::draw() 
{
    if (!isLoaded)
        return;

    // Draw points
    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glDrawElements(GL_POINTS, num_pts * 3, GL_UNSIGNED_SHORT, NULL);
}