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
#include "../utils/LoadShaders.h"
#include "../utils/TextureController.h"
#include "../utils/Texture.h"

class SubWindow : public Models
{
    public:
        SubWindow();
        ~SubWindow();

        void draw();
        void setModelMatrix(glm::mat4 nmodel);
        const glm::mat4 getModelMatrix();

        void setSize(float h, float w);

        void loadTexture(sf::Image texture);
    protected:
    private:
        static int counter;
        static const int OFFSET = 30;
        glm::mat4 model;

        TextureController* texCtrl = NULL;
        Texture* texture = NULL;

        void update(const char * data);       ///< Implementation of observer pattern.
};

#endif // SubWindow_H
