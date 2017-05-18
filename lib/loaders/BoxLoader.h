#ifndef BOXLOADER_H
#define BOXLOADER_H

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/LoadShaders.h"
#include "../utils/ProgramDefines.h"
#include "../objects/BoundingBox.h"

class BoxLoader
{
    public:
        static BoxLoader* instance();
        virtual ~BoxLoader();
        void draw(GLuint PVMLoc, glm::mat4 projection, glm::mat4 view);
        void LoadDataToGraphicsCard();
        void loadBoxes(std::vector<BoundingBox> v);
    protected:

    private:
        BoxLoader();
        static const int NUM_PTS = 8;
        std::vector<float> data;
        std::vector<BoundingBox> boxes;

        GLfloat points[8 * 4] = {
            -0.5, -0.5, -0.5, 1.0,
             0.5, -0.5, -0.5, 1.0,
             0.5,  0.5, -0.5, 1.0,
            -0.5,  0.5, -0.5, 1.0,
            -0.5, -0.5,  0.5, 1.0,
             0.5, -0.5,  0.5, 1.0,
             0.5,  0.5,  0.5, 1.0,
            -0.5,  0.5,  0.5, 1.0
        };            ///< Cube 1x1x1, centered on origin

        GLushort indices[16] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            0, 4, 1, 5,
            2, 6, 3, 7
        };

        GLfloat colors[NUM_PTS * 3];

        GLuint vboptr;  ///< ID for faces VBO.
        GLuint eboptr;  ///< ID for faces index array.
        GLuint bufptr;  ///< ID for faces array buffer.

        void learBoxes();
};

#endif // BOXLOADER_H
