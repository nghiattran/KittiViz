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
#include "../utils/Texture.h"

class TextureController
{
    public:
        static TextureController* getInstance();
        ~TextureController();

        Texture* createNewTexture(sf::Image texture);
        Texture* updateTexture(Texture* texture, sf::Image img);
        void turnTextureOff(Texture* tex);
        void turnTextureOn(Texture* tex);
        void setModelMatrix(glm::mat4 model);
        static sf::Image loadTextureFromFile(const char* filename);
        void useProgram();
    protected:

    private:
        static TextureController* mInstance;

        TextureController();

        static int cnt;
        static const int TEXTURE_NUM = 10;
        GLuint texID[TEXTURE_NUM];

        GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.
        GLuint texTransLoc;    ///< Location ID of the texture transformation matrix in the shader.
        GLuint useTextureLoc;  ///< Location ID of the texture use flag in the shader.
        GLuint NormalLoc;
        GLuint ModelLoc;
        GLuint program;        ///< ID of the shader program.
};

#endif // TEXTURECONTROLLER_H
