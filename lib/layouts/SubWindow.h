#ifndef SubWindow_H
#define SubWindow_H


#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Models.h"
#include "../utils/TextureController.h"

class SubWindow : public TextureController, Models
{
    public:
        SubWindow();
        ~SubWindow();

        void draw();
        const glm::mat4 getModelMatrix();
        void setModelMatrix(glm::mat4 nmodel) override;
        void setSize(float h, float w);
    protected:
    private:

        void update(const char * data);       ///< Implementation of observer pattern.
};
#endif // SubWindow_H
