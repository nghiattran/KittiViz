#include "TextureController.h"

TextureController* TextureController::mInstance = NULL;
int TextureController::cnt = 0;

TextureController::TextureController()
{
    //  Load the shaders
    program = LoadShadersFromFile("Shaders/VertexShaderLightingTextureEnvMap.glsl", "Shaders/PhongMultipleLightsAndTextureEnvMap.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "cmtex"), 0);

    PVMLoc = glGetUniformLocation(program, "PVM");
    ModelLoc = glGetUniformLocation(program, "Model");
    NormalLoc = glGetUniformLocation(program, "NormalMatrix");
    texTransLoc = glGetUniformLocation(program, "textrans");

    glGenTextures(5, texID);

    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

    // float scaleFactor = 0.1;
    // setModelMatrix(glm::scale(glm::mat4(1.0), glm::vec3(scaleFactor, scaleFactor, scaleFactor)));
}

TextureController::~TextureController()
{
    //dtor
}


TextureController* TextureController::getInstance()
{
    if (!mInstance)
    {
        mInstance = new TextureController();
    }
    return mInstance;
}

/**
\brief Load texture to subwindow from file.

\parame filename - name of texture file.
*/

sf::Image TextureController::loadTextureFromFile(const char* filename)
{
    sf::Image texture;
    bool texloaded = texture.loadFromFile(filename);

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    return texture;
}

Texture* TextureController::createNewTexture(sf::Image img)
{
    glUseProgram(program);
    Texture* texture = new Texture(program, cnt);

    updateTexture(texture, img);

    cnt++;
    return texture;
}

Texture* TextureController::updateTexture(Texture* texture, sf::Image img)
{
    glUseProgram(program);

    GLuint texLoc = texture->getTextureLoc();
    int index = texture->getIndex();

    glUniform1i(texLoc, texID[index]);

    //  Load the texture into texture memory.
    glActiveTexture(GL_TEXTURE0+texID[index]);
    glBindTexture(GL_TEXTURE_2D, texID[index]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

/**
\brief Turns a texture off.

\param tex --- The texture to be turned off.

*/

void TextureController::turnTextureOff(Texture* tex)
{
    glUseProgram(program);
    glUniform1i(tex->getUseTextureLoc(), false);
}

/**
\brief Turns a texture on.

\param tex --- The texture to be turned on.

*/

void TextureController::turnTextureOn(Texture* tex)
{
    glUseProgram(program);
    glUniform1i(tex->getUseTextureLoc(), true);
}

/**
\brief Set model matrix.

\param model --- The model matrix.

*/

void TextureController::setModelMatrix(glm::mat4 model)
{
    glUseProgram(program);
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(model));
}

/**
\brief Set model matrix.

*/

void TextureController::useProgram()
{
    glUseProgram(program);
}
