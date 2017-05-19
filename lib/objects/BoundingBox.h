#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/LoadShaders.h"
#include "../utils/ProgramDefines.h"

class BoundingBox
{
    public:
        BoundingBox(char[], glm::vec3, glm::vec3, float);
        BoundingBox(char[], glm::vec3, glm::vec3, glm::vec3);
        ~BoundingBox();
        glm::mat4 getModelMatrix();
    protected:
    private:
        glm::mat4 model;
        std::string objectType;
        glm::vec3 size;
        glm::vec3 transform;
        float rotAngle;

        void setData(char[], glm::vec3, glm::vec3, float);
};

#endif // BOUNDINGBOX_H
