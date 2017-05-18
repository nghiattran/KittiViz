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

class SubWindow : public Models
{
    public:
        SubWindow();
        ~SubWindow();

        void draw();
        void setLanes(int num);
        void transformTexture(bool isScolling);
        void detransformTexture();

        void setDrawingText(bool val);

        void setModelMatrix(glm::mat4 nmodel);
        const glm::mat4 getModelMatrix();

        void setSize(float h, float w);

        void loadTextureFromFile(const char* filename);
        void loadTexture(sf::Image texture);
    protected:
    private:
        GLuint texID[6];      ///< Texture IDs.
        int lanes;
        int cnt;

        GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.
        GLuint texTransLoc;    ///< Location ID of the texture transformation matrix in the shader.
        GLuint useTextureLoc;  ///< Location ID of the texture use flag in the shader.
        GLuint program;        ///< ID of the shader program.

        glm::mat4 projection;  ///< Projection Matrix
        glm::mat4 model;       ///< Model Matrix
        glm::mat4 textrans;    ///< Texture transformation matrix.

        GLboolean isTexture;   ///< Boolean for drawing texture

        void update(const char * data);       ///< Implementation of observer pattern.
};
#endif // SubWindow_H
