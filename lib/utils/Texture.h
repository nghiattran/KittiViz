#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

class Texture
{
    public:
        Texture(GLuint& program, int i) {
            glUseProgram(program);
            char tex[10];
            sprintf(tex, "tex[%d]", i);

            char useTex[20];
            sprintf(useTex, "useTexture[%d]", i);

            textureLoc = glGetUniformLocation(program, tex);
            useTextureLoc = glGetUniformLocation(program, useTex);
            index = i;
        }
        ~Texture() {
        };

        int getIndex() {
            return index;
        }

        GLuint getTextureLoc() {
            return textureLoc;
        }

        GLuint getUseTextureLoc() {
            return useTextureLoc;
        }
    private:
        int index;
        GLuint textureLoc;
        GLuint useTextureLoc;
};

#endif // TEXTURE_H
