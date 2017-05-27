#ifndef POINTSLOADER_H
#define POINTSLOADER_H

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include <iostream>
#include <string>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../patterns/Observer.h"
#include "../utils/ProgramDefines.h"
#include "../utils/LoadShaders.h"
#include "../data/CloudPoints.h"

class PointsLoader : public Observer<CloudPoints>
{
    public:
        ~PointsLoader();
        static PointsLoader* getInstance();

        void draw();
        void update(CloudPoints);

        void LoadDataToGraphicsCard(std::vector<float>);
    protected:
    private:
        PointsLoader();

        static PointsLoader* mInstance;
        bool isLoaded;

        static constexpr float maxVelodyneDst = 80;     ///< Approximation of max distance of points in KITTI velodyne setup.

        GLuint num_pts;

        GLuint vboptr;  ///< ID for faces VBO.
        GLuint eboptr;  ///< ID for faces index array.
        GLuint bufptr;  ///< ID for faces array buffer.
};

#endif // POINTSLOADER_H
