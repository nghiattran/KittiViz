#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
\file Material.h
\brief Header file for Material.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class Material

\brief The Material class is simply a class for holding Phong model material properties.

*/

class Material
{
private:
    glm::vec4 ambient;   ///< Ambient color of the material.
    glm::vec4 diffuse;   ///< Diffuse color of the material.
    glm::vec4 specular;  ///< Specular color of the material.
    glm::vec4 emission;  ///< Emission color of the material.
    float shininess;     ///< Shininess exponent of the material.

public:
    Material();
    Material(glm::vec4 a,
             glm::vec4 d,
             glm::vec4 s,
             glm::vec4 e,
             float sh
            );

    Material(float ar, float ag, float ab, float aa,
             float dr, float dg, float db, float da,
             float sr, float sg, float sb, float sa,
             float er, float eg, float eb, float ea,
             float sh
            );

    ~Material();

    void setMaterial(glm::vec4 a,
                     glm::vec4 d,
                     glm::vec4 s,
                     glm::vec4 e,
                     float sh
                    );

    void setMaterial(float ar, float ag, float ab, float aa,
                     float dr, float dg, float db, float da,
                     float sr, float sg, float sb, float sa,
                     float er, float eg, float eb, float ea,
                     float sh
                    );

    glm::vec4 getAmbient();
    glm::vec4 getDiffuse();
    glm::vec4 getSpecular();
    glm::vec4 getEmission();
    float getShininess();

    void setAmbient(glm::vec4 a);
    void setDiffuse(glm::vec4 d);
    void setSpecular(glm::vec4 s);
    void setEmission(glm::vec4 e);
    void setShininess(float sh);

    void setAmbient(float ar, float ag, float ab, float aa);
    void setDiffuse(float dr, float dg, float db, float da);
    void setSpecular(float sr, float sg, float sb, float sa);
    void setEmission(float er, float eg, float eb, float ea);

};

#endif // MATERIAL_H_INCLUDED
