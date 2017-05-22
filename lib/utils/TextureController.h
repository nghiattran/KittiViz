#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

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

#include "../utils/LoadShaders.h"
#include "../utils/ProgramDefines.h"

class TextureController
{
    public:
        TextureController();
        ~TextureController();

        virtual void setModelMatrix(glm::mat4 model);
        const glm::mat4 getModelMatrix();

        void loadTextureFromFile(const char* filename);
        void loadTexture(sf::Image texture);
        
        void useProgram();

        void turnOnTexture();
        void turnOffTexture();
    protected:
        glm::mat4 model;
    private:
        GLuint tex1Loc;        ///< Location ID of the texture in the shader.
        GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.

        GLuint texTransLoc;    ///< Location ID of the texture transformation matrix in the shader.
        GLuint useTextureLoc;  ///< Location ID of the texture use flag in the shader.
        GLuint program;        ///< ID of the shader program.

        GLuint texID;
};

#endif // TEXTURECONTROLLER_H
