#include "TextureController.h"

TextureController::TextureController()
{
    //  Load the shaders
    program = LoadShadersFromFile("Shaders/VertexShaderBasicTexture.glsl", "Shaders/SimpleTexture.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    PVMLoc = glGetUniformLocation(program, "PVM");
    useTextureLoc = glGetUniformLocation(program, "useTexture");
    texTransLoc = glGetUniformLocation(program, "textrans");
    tex1Loc = glGetUniformLocation(program, "tex1");

    glm::mat4 model = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(useTextureLoc, true);
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

    glGenTextures(1, &texID);
    turnOnTexture();

    // Load placeholder texture
    loadTextureFromFile("Textures/Repeat-brick.jpg");
}


TextureController::~TextureController()
{
    //dtor
}

/**
\brief Load texture to subwindow from file.
\parame filename - name of texture file.
*/

void TextureController::loadTextureFromFile(const char* filename)
{
    sf::Image texture;
    bool texloaded = texture.loadFromFile(filename);

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    loadTexture(texture);
}

/**
\brief Load texture.
\parame texture - texture object.
*/

void TextureController::loadTexture(sf::Image texture)
{
    glUseProgram(program);

    //  Link the texture to the shader.
    GLuint tex1_uniform_loc = glGetUniformLocation(program, "tex1");
    glUniform1i(tex1Loc, texID);

    //  Load the texture into texture memory.
    glActiveTexture(GL_TEXTURE0 + texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
\brief Set model matrix.

\param model --- The model matrix.

*/

void TextureController::setModelMatrix(glm::mat4 nmodel)
{
    model = nmodel;
    glUseProgram(program);
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(nmodel));
}


/**
\brief Get model matrix.
*/

const glm::mat4 TextureController::getModelMatrix()
{
    return model;
}

/**
\brief Activate program.

*/

void TextureController::useProgram()
{
    glUseProgram(program);
}

/**
\brief Turn on texture for this program.

*/

void TextureController::turnOnTexture()
{
    glUseProgram(program);
    glUniform1i(useTextureLoc, true);
}

/**
\brief Turn off texture for this program.

*/

void TextureController::turnOffTexture()
{
    glUseProgram(program);
    glUniform1i(useTextureLoc, false);
}
