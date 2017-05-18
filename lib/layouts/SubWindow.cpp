#include "SubWindow.h"

SubWindow::SubWindow()
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

    glm::mat4 model = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(model));

    loadTextureFromFile("Textures/Repeat-brick.jpg");

    createTessellatedWallOBJ(1, 1, 1, 1);
    load(0, 1, 2, 3);
    setBorderColor(1, 1, 1);
    setDrawBorder(true);
    glUniform1i(useTextureLoc, true);
}

SubWindow::~SubWindow()
{
    //dtor
}

/**
\brief Set number of lanes.

\parame int - number of lanes
*/

void SubWindow::setLanes(int num)
{
    lanes = num;
}

/**
\brief Draw road.

\param nprogram - program that is in used before drawing texture. This make sure everything is reset after draw texture.
*/

void SubWindow::draw()
{
    glUseProgram(program);
    Models::draw();
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
}

/**
\brief Tranform texture

*/

void SubWindow::transformTexture(bool isScolling)
{
    if (isScolling)
        cnt += 1;

    textrans = glm::rotate(textrans, -180*degf, glm::vec3(0, 0, 1));
    textrans = glm::translate(textrans, glm::vec3(0.01 * cnt, 0, 0));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
}

/**
\brief Detranform texture

*/

void SubWindow::detransformTexture()
{
    textrans = glm::translate(textrans, glm::vec3(-0.01 * cnt, 0, 0) );
    textrans = glm::rotate(textrans, 180*degf, glm::vec3(0, 0, 1));
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
}

/**
\brief Set model matrix.

\param nmodel - target matrix.

*/

void SubWindow::setModelMatrix(glm::mat4 nmodel)
{
    glUseProgram(program);
    // Flip image around x axis, to fix flipping error
    model = glm::rotate(nmodel, 180*degf, glm::vec3(1, 0, 0));
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*model));
}

/**
\brief Get model matrix.
*/

const glm::mat4 SubWindow::getModelMatrix()
{
    return model;
}


/**
\brief Set size.

\parame int - number of lanes
*/

void SubWindow::setSize(float h, float w)
{
    createTessellatedWallOBJ(h, w, 1, 1);
    load(0, 1, 2, 3);
}

/**
\brief Load texture to subwindow from file.

\parame filename - name of texture file.
*/

void SubWindow::loadTextureFromFile(const char* filename)
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

void SubWindow::loadTexture(sf::Image texture)
{
    glUseProgram(program);

    GLuint texID;
    glGenTextures(1, &texID);

    //  Link the texture to the shader.
    GLuint tex1_uniform_loc = glGetUniformLocation(program, "tex1");
    glUniform1i(tex1_uniform_loc, 0);

    //  Load the texture into texture memory.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}