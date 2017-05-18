#ifndef OBJMODEL_H_INCLUDED
#define OBJMODEL_H_INCLUDED

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/Graphics.hpp>  // Used only for image loading.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../utils/LoadShaders.h"
#include "../utils/ProgramDefines.h"
#include "../utils/Material.h"
#include "../objects/Light.h"

/**
\file ObjModel.h
\brief Header file for ObjModel.cpp

\author    Don Spickler
\version   1.2
\date      Written: 4/10/2016  <BR> Revised: 3/20/2017

*/

/**
\class ObjModel

\brief Model loader class for simple Wavefront obj files.

Loads in a simple Wavefront obj file, one that contains vertex, normal, and
texture coordinate data.  The vertex data must be in triangulated form.  This
version also reads in material data and segments face groups by material type,
including textures.

Tags that are supported are the standard ambient, diffuse, specular, emission,
and shininess exponent in addition to textures, that is, Ka, Kd, Ks, Ke, Ns,
map_Ka, map_Kd, and map_Ks.

This version uses its own shader programs in order to keep the loading and graphing
of the models encapsulated, there is one shader program for each segment of the model.
It is also designed to be easily manipulated from an external method.  Since this
version uses its own shaders, lights, along with the projection, viewing, and model
matrices must be loaded from an external controling method.

Code was altered from Tutorial 7 : Model loading from http://www.opengl-tutorial.org/.

*/

struct objMaterial
{
    std::string name;
    Material mat;
    std::string ATextureFile;
    std::string DTextureFile;
    std::string STextureFile;
};

struct objTexture
{
    std::string Filename;
    sf::Image Texture;
};

#define MaxLights 10

class ObjModel
{
private:
    GLuint vboptr;  ///< ID for the VBO.
    GLuint bufptr;  ///< ID for the array buffer.

    std::vector<GLuint> VBOs;               ///< Vector for storing the VBO addresses.
    std::vector<std::string> MatNames;      ///< Vector for storing the Material names.
    std::vector<unsigned int> VertexSizes;  ///< Vector for storing the vertex set sizes.

    glm::mat4 projection;  ///< Projection Matrix
    glm::mat4 view;        ///< View Matrix
    glm::mat4 model;       ///< Model Matrix

    std::vector<glm::vec3> vertices;  ///< Vector for storing the vertex data.
    std::vector<glm::vec2> texcoords; ///< Vector for storing the texture coordinate data.
    std::vector<glm::vec3> normals;   ///< Vector for storing the normal vector data.

    std::vector<objMaterial> mats;   ///< Vector for storing the object materials.
    std::vector<objTexture> textures;   ///< Vector for storing the object textures.

    std::vector<GLuint> programs;   ///< Vector for storing the GLSL program addresses.

    Material mat;         ///< Default material for the object.
    Light lt[MaxLights];  ///< Light object.
    int numLights;        ///< Number of lights.

    glm::vec4 GlobalAmbient;  ///< Global Ambient for lighting.
    glm::vec3 eye;            ///< Camera position.

    float texRatio;   ///< Material texture ratio, r to texture and (1-r) to material.

    void LoadDataToGraphicsCard(std::string matname);

    void turnLightOn(GLuint program, std::string name, int i);
    void turnLightOff(GLuint program, std::string name, int i);
    void LoadLights(GLuint program, Light Lt[], std::string name, int num);
    void LoadLight(GLuint program, Light Lt, std::string name, int i);

    std::string RemovePath(std::string filename);

    void setViewMatrix(GLuint program, glm::mat4 viewMatrix);
    void setProjectionMatrix(GLuint program, glm::mat4 projectionMatrix);
    void setModelMatrix(GLuint program, glm::mat4 modelMatrix);
    void setEye(GLuint program, glm::vec3 Eye);

public:
    ObjModel();
    ~ObjModel();

    bool Load(std::string path, std::string filename);
    bool LoadMateials(std::string path, std::string filename);
    bool LoadTextures(std::string path);
    bool LoadPrograms();

    void loadMaterial();
    void loadMaterial(Material Mat);
    void loadMaterial(GLuint program, Material mat);
    void setMaterial(Material Mat);

    void loadTexture(GLuint program, sf::Image tex, std::string name);


    void setNumberOfLights(int num);
    void setLight(int num, Light light);
    Light getLight(int num);
    void setGlobalAmbient(glm::vec4 ga);

    void turnLightOn(int num);
    void turnLightOff(int num);

    void turnLightOnOff(int num, bool setting);

    void setViewMatrix(glm::mat4 viewMatrix);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setModelMatrix(glm::mat4 modelMatrix);
    void setEye(glm::vec3 Eye);

    void setTextureRatio(float texR);

    void draw();
    void draw(glm::mat4 p, glm::mat4 v);
};

#endif // OBJMODEL_H_INCLUDED
