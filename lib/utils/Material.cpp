#include "Material.h"


/**
\file Material.cpp
\brief Implementation of the Material class.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor, default.

*/

Material::Material(){}


/**
\brief Sets all of the material attributes.

\param a --- Ambient color of the material.
\param d --- Diffuse color of the material.
\param s --- Specular color of the material.
\param e --- Emission color of the material.
\param sh --- Shininess exponent of the material.

*/

Material::Material(glm::vec4 a,
                   glm::vec4 d,
                   glm::vec4 s,
                   glm::vec4 e,
                   float sh
                  )
{
    ambient = a;
    diffuse = d;
    specular = s;
    emission = e;
    shininess = sh;
}


/**
\brief Sets all of the material attributes using floats.

\param ar, ag, ab, aa --- Ambient color of the light.
\param dr, dg, db, da --- Diffuse color of the light.
\param sr, sg, sb, sa --- Specular color of the light.
\param er, eg, eb, ea --- Emission color of the light.
\param sh --- Shininess exponent of the material.

*/

Material::Material(float ar, float ag, float ab, float aa,
                   float dr, float dg, float db, float da,
                   float sr, float sg, float sb, float sa,
                   float er, float eg, float eb, float ea,
                   float sh
                  )
{
    ambient = glm::vec4(ar, ag, ab, aa);
    diffuse = glm::vec4(dr, dg, db, da);
    specular = glm::vec4(sr, sg, sb, sa);
    emission = glm::vec4(er, eg, eb, ea);
    shininess = sh;
}


/**
\brief Sets all of the material attributes.

\param a --- Ambient color of the material.
\param d --- Diffuse color of the material.
\param s --- Specular color of the material.
\param e --- Emission color of the material.
\param sh --- Shininess exponent of the material.

*/

void Material::setMaterial(glm::vec4 a,
                           glm::vec4 d,
                           glm::vec4 s,
                           glm::vec4 e,
                           float sh
                          )
{
    ambient = a;
    diffuse = d;
    specular = s;
    emission = e;
    shininess = sh;
}

/**
\brief Sets all of the material attributes using floats.

\param ar, ag, ab, aa --- Ambient color of the light.
\param dr, dg, db, da --- Diffuse color of the light.
\param sr, sg, sb, sa --- Specular color of the light.
\param er, eg, eb, ea --- Emission color of the light.
\param sh --- Shininess exponent of the material.

*/

void Material::setMaterial(float ar, float ag, float ab, float aa,
                           float dr, float dg, float db, float da,
                           float sr, float sg, float sb, float sa,
                           float er, float eg, float eb, float ea,
                           float sh
                          )
{
    ambient = glm::vec4(ar, ag, ab, aa);
    diffuse = glm::vec4(dr, dg, db, da);
    specular = glm::vec4(sr, sg, sb, sa);
    emission = glm::vec4(er, eg, eb, ea);
    shininess = sh;
}

/**
\brief Destructor

No implementation needed at this point.

*/

Material::~Material(){}

/**
\brief Returns the ambient color vector of the material.

*/

glm::vec4 Material::getAmbient()
{
    return ambient;
}

/**
\brief Returns the diffuse color vector of the material.

*/

glm::vec4 Material::getDiffuse()
{
    return diffuse;
}

/**
\brief Returns the specular color vector of the material.

*/

glm::vec4 Material::getSpecular()
{
    return specular;
}

/**
\brief Returns the emission color vector of the material.

*/

glm::vec4 Material::getEmission()
{
    return emission;
}

/**
\brief Returns the shininess exponent of the material.

*/

float Material::getShininess()
{
    return shininess;
}


/**
\brief Sets the ambient material color.

\param a --- Ambient color of the material.

*/

void Material::setAmbient(glm::vec4 a)
{
    ambient = a;
}

/**
\brief Sets the diffuse material color.

\param d --- Diffuse color of the material.

*/

void Material::setDiffuse(glm::vec4 d)
{
    diffuse = d;
}


/**
\brief Sets the specular material color.

\param s --- Specular color of the material.

*/

void Material::setSpecular(glm::vec4 s)
{
    specular = s;
}


/**
\brief Sets the emission material color.

\param e --- Emission color of the material.

*/

void Material::setEmission(glm::vec4 e)
{
    emission = e;
}

/**
\brief Sets the shininess exponent of the material.

\param sh --- Shininess exponent.

*/

void Material::setShininess(float sh)
{
    shininess = sh;
}

/**
\brief Sets all of the ambient attributes using floats.

\param ar, ag, ab, aa --- Ambient color of the light.

*/

void Material::setAmbient(float ar, float ag, float ab, float aa)
{
    ambient = glm::vec4(ar, ag, ab, aa);
}

/**
\brief Sets all of the diffuse attributes using floats.

\param dr, dg, db, da --- Diffuse color of the light.

*/

void Material::setDiffuse(float dr, float dg, float db, float da)
{
    diffuse = glm::vec4(dr, dg, db, da);
}

/**
\brief Sets all of the specular attributes using floats.

\param sr, sg, sb, sa --- Specular color of the light.

*/

void Material::setSpecular(float sr, float sg, float sb, float sa)
{
    specular = glm::vec4(sr, sg, sb, sa);
}

/**
\brief Sets all of the emission attributes using floats.

\param er, eg, eb, ea --- Emission color of the light.

*/

void Material::setEmission(float er, float eg, float eb, float ea)
{
    emission = glm::vec4(er, eg, eb, ea);
}
