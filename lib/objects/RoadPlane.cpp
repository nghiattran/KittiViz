#include "RoadPlane.h"

RoadPlane::RoadPlane()
{
    //ctor
    // setDrawBorder(false);

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

    sf::Image texture;
    bool texloaded = texture.loadFromFile("Textures/Repeat-brick.jpg");

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

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

    createTessellatedWallOBJ(0.5, 0.5, 1, 2);
    Models::load(0, 1, 2, 3);
}

RoadPlane::~RoadPlane()
{
    //dtor
}

/**
\brief Set number of lanes.

\parame int - number of lanes
*/

void RoadPlane::setLanes(int num) {
    lanes = num;
}

/**
\brief Draw road.

\param nprogram - program that is in used before drawing texture. This make sure everything is reset after draw texture.
*/

void RoadPlane::draw(GLuint& nprogram) {
    glUseProgram(program);
    glm::mat4 view(1.0);
    glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(projection*view*model));
    
    glUniform1i(useTextureLoc, true);
    Models::draw();
    glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
    glUniform1i(useTextureLoc, false);

    glUseProgram(nprogram);
}

/**
\brief Draws the box to the screen.

*/

void RoadPlane::draw(GLuint PVMLoc, glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
  
}

/**
\brief Tranform texture

*/

void RoadPlane::transformTexture(bool isScolling)
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

void RoadPlane::detransformTexture()
{
  textrans = glm::translate(textrans, glm::vec3(-0.01 * cnt, 0, 0) );
  textrans = glm::rotate(textrans, 180*degf, glm::vec3(0, 0, 1));
  glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, glm::value_ptr(textrans));
}