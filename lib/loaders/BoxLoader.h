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
#include "../data/BoundingBox.h"
#include "../data/BoxList.h"
#include "../patterns/Observer.h"

class BoxLoader : public Observer<BoxList>
{
    public:
        static BoxLoader* instance();
        virtual ~BoxLoader();
        void draw(GLuint PVMLoc, glm::mat4 projection, glm::mat4 view);
        void LoadDataToGraphicsCard();
        void update(BoxList v);
    protected:

    private:
        BoxLoader();
        static BoxLoader* mInstance;

        static const int NUM_PTS = 12;
        std::vector<float> data;
        std::vector<BoundingBox> boxes;

        GLfloat points[NUM_PTS * 4] = {
            -0.5, -0.5, -0.5, 1.0,
            -0.5, -0.5,  0.5, 1.0,
            -0.5,  0.5,  0.5, 1.0,
            -0.5,  0.5, -0.5, 1.0,

            -0.3, -0.5, -0.5, 1.0,
            -0.3, -0.5,  0.5, 1.0,
            -0.3,  0.5,  0.5, 1.0,
            -0.3,  0.5, -0.5, 1.0,

            0.5, -0.5, -0.5, 1.0,
            0.5, -0.5,  0.5, 1.0,
            0.5,  0.5,  0.5, 1.0,
            0.5,  0.5, -0.5, 1.0,
        };            ///< Cube 1x1x1, centered on origin

        GLushort indices[32] = {
            0, 1, 2, 3,
            4, 5, 6, 7,
            0, 4, 1, 5,
            2, 6, 3, 7,

            4, 5, 6, 7,
            8, 9, 10, 11,
            4, 8, 5, 9,
            6, 10, 7, 11
        };

        GLfloat colors[NUM_PTS * 3];

        GLuint vboptr;  ///< ID for faces VBO.
        GLuint eboptr;  ///< ID for faces index array.
        GLuint bufptr;  ///< ID for faces array buffer.

        void learBoxes();
};

#endif // BOXLOADER_H
