#include "Light.h"

/**
\file Light.cpp
\brief Implementation of the Light class.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor, default.

*/

Light::Light() {}

/**
\brief Constructor

\param onoff --- Light on or off.
\param pos --- Position of the light.
\param spotDir --- Direction of the spot light.
\param a --- Ambient color of the light.
\param d --- Diffuse color of the light.
\param s --- Specular color of the light.
\param spotCut --- Spot cutoff angle.
\param spotExp --- Spot falloff exponent.
\param atten --- Attenuation vector, x = constant, y = linear, z = quadratic.

Sets all of the light attributes.

*/

Light::Light(bool onoff,
             glm::vec4 pos,
             glm::vec3 spotDir,
             glm::vec4 a,
             glm::vec4 d,
             glm::vec4 s,
             float spotCut,
             float spotExp,
             glm::vec3 atten
            )
{
    on = onoff;
    position = pos;
    spotDirection = spotDir;
    ambient = a;
    diffuse = d;
    specular = s;
    spotCutoff = spotCut;
    spotExponent = spotExp;
    attenuation = atten;
}


/**
\brief Constructor

\param onoff --- Light on or off.
\param posx, posy, posz, posw --- Position of the light.
\param spotDirx, spotDiry, spotDirz --- Direction of the spot light.
\param ar, ag, ab, aa --- Ambient color of the light.
\param dr, dg, db, da --- Diffuse color of the light.
\param sr, sg, sb, sa --- Specular color of the light.
\param spotCut --- Spot cutoff angle.
\param spotExp --- Spot falloff exponent.
\param attenc, attenl, attenq --- Attenuation vector, attenc = constant, attenl = linear, attenq = quadratic.

Sets all of the light attributes using float and boolean input.

*/

Light::Light(bool onoff,
             float posx, float posy, float posz, float posw,
             float spotDirx, float spotDiry, float spotDirz,
             float ar, float ag, float ab, float aa,
             float dr, float dg, float db, float da,
             float sr, float sg, float sb, float sa,
             float spotCut,
             float spotExp,
             float attenc, float attenl, float attenq
            )
{
    on = onoff;
    position = glm::vec4(posx, posy, posz, posw);
    spotDirection = glm::vec3(spotDirx, spotDiry, spotDirz);
    ambient = glm::vec4(ar, ag, ab, aa);
    diffuse = glm::vec4(dr, dg, db, da);
    specular = glm::vec4(sr, sg, sb, sa);
    spotCutoff = spotCut;
    spotExponent = spotExp;
    attenuation = glm::vec3(attenc, attenl, attenq);
}


/**
\brief Sets all of the light attributes.

\param onoff --- Light on or off.
\param pos --- Position of the light.
\param spotDir --- Direction of the spot light.
\param a --- Ambient color of the light.
\param d --- Diffuse color of the light.
\param s --- Specular color of the light.
\param spotCut --- Spot cutoff angle.
\param spotExp --- Spot falloff exponent.
\param atten --- Attenuation vector, x = constant, y = linear, z = quadratic.

*/

void Light::setLight(bool onoff,
                     glm::vec4 pos,
                     glm::vec3 spotDir,
                     glm::vec4 a,
                     glm::vec4 d,
                     glm::vec4 s,
                     float spotCut,
                     float spotExp,
                     glm::vec3 atten
                    )
{
    on = onoff;
    position = pos;
    spotDirection = spotDir;
    ambient = a;
    diffuse = d;
    specular = s;
    spotCutoff = spotCut;
    spotExponent = spotExp;
    attenuation = atten;
}


/**
\brief Sets all of the light attributes using float and boolean input.

\param onoff --- Light on or off.
\param posx, posy, posz, posw --- Position of the light.
\param spotDirx, spotDiry, spotDirz --- Direction of the spot light.
\param ar, ag, ab, aa --- Ambient color of the light.
\param dr, dg, db, da --- Diffuse color of the light.
\param sr, sg, sb, sa --- Specular color of the light.
\param spotCut --- Spot cutoff angle.
\param spotExp --- Spot falloff exponent.
\param attenc, attenl, attenq --- Attenuation vector, attenc = constant, attenl = linear, attenq = quadratic.

*/

void Light::setLight(bool onoff,
                     float posx, float posy, float posz, float posw,
                     float spotDirx, float spotDiry, float spotDirz,
                     float ar, float ag, float ab, float aa,
                     float dr, float dg, float db, float da,
                     float sr, float sg, float sb, float sa,
                     float spotCut,
                     float spotExp,
                     float attenc, float attenl, float attenq
                    )
{
    on = onoff;
    position = glm::vec4(posx, posy, posz, posw);
    spotDirection = glm::vec3(spotDirx, spotDiry, spotDirz);
    ambient = glm::vec4(ar, ag, ab, aa);
    diffuse = glm::vec4(dr, dg, db, da);
    specular = glm::vec4(sr, sg, sb, sa);
    spotCutoff = spotCut;
    spotExponent = spotExp;
    attenuation = glm::vec3(attenc, attenl, attenq);
}


/**
\brief Destructor

No implementation needed at this point.

*/

Light::~Light() {}


/**
\brief Returns the on or off status of the light.

*/

bool Light::getOn()
{
    return on;
}

/**
\brief Returns the position of the light.

*/

glm::vec4 Light::getPosition()
{
    return position;
}

/**
\brief Returns the spot direction of the light.

*/

glm::vec3 Light::getSpotDirection()
{
    return spotDirection;
}

/**
\brief Returns the ambient color of the light.

*/

glm::vec4 Light::getAmbient()
{
    return ambient;
}

/**
\brief Returns the diffuse color of the light.

*/

glm::vec4 Light::getDiffuse()
{
    return diffuse;
}

/**
\brief Returns the specular color of the light.

*/

glm::vec4 Light::getSpecular()
{
    return specular;
}

/**
\brief Returns the spot cutoff of the light.

*/

float Light::getSpotCutoff()
{
    return spotCutoff;
}

/**
\brief Returns the spot exponent of the light.

*/

float Light::getSpotExponent()
{
    return spotExponent;
}

/**
\brief Returns the attenuation vector of the light.

*/

glm::vec3 Light::getAttenuation()
{
    return attenuation;
}

/**
\brief Sets the on for the light to the value of b.

\param b --- true for on and false for off.

*/

void Light::setOn(bool b)
{
    on = b;
}

/**
\brief Sets the position of the light.

\param p --- New position of the light.

*/

void Light::setPosition(glm::vec4 p)
{
    position = p;
}

/**
\brief Sets the spot direction of the light.

\param sd --- New spot direction of the light.

*/

void Light::setSpotDirection(glm::vec3 sd)
{
    spotDirection = sd;
}

/**
\brief Sets the ambient color of the light.

\param a --- New ambient color of the light.

*/

void Light::setAmbient(glm::vec4 a)
{
    ambient = a;
}

/**
\brief Sets the diffuse color of the light.

\param d --- New diffuse color of the light.

*/

void Light::setDiffuse(glm::vec4 d)
{
    diffuse = d;
}

/**
\brief Sets the specular color of the light.

\param s --- New specular color of the light.

*/

void Light::setSpecular(glm::vec4 s)
{
    specular = s;
}

/**
\brief Sets the spot cutoff of the light.

\param sc --- New spot cutoff of the light.

*/

void Light::setSpotCutoff(float sc)
{
    spotCutoff = sc;
}

/**
\brief Sets the spot exponent of the light.

\param se --- New spot exponent of the light.

*/

void Light::setSpotExponent(float se)
{
    spotExponent = se;
}

/**
\brief Sets the spot attenuation vector of the light.

\param se --- New spot attenuation vector of the light.

*/

void Light::setAttenuation(glm::vec3 atten)
{
    attenuation = atten;
}

/**
\brief Sets the position of the light.

\param px --- New position of the light, x coordinate.

\param py --- New position of the light, y coordinate.

\param pz --- New position of the light, z coordinate.

\param pw --- New position of the light, w coordinate.

*/

void Light::setPosition(float px, float py, float pz, float pw)
{
    position = glm::vec4(px, py, pz, pw);
}


/**
\brief Sets the spot direction of the light.

\param sdx --- New spot direction of the light, x coordinate.

\param sdy --- New spot direction of the light, y coordinate.

\param sdz --- New spot direction of the light, z coordinate.

\param sdw --- New spot direction of the light, w coordinate.

*/

void Light::setSpotDirection(float sdx, float sdy, float sdz)
{
    spotDirection = glm::vec3(sdx, sdy, sdz);
}


/**
\brief Sets the ambient color of the light.

\param ar --- New ambient color of the light, red component.

\param ag --- New ambient color of the light, green component.

\param ab --- New ambient color of the light, blue component.

\param aa --- New ambient color of the light, alpha component.

*/

void Light::setAmbient(float ar, float ag, float ab, float aa)
{
    ambient = glm::vec4(ar, ag, ab, aa);
}

/**
\brief Sets the diffuse color of the light.

\param dr --- New diffuse color of the light, red component.

\param dg --- New diffuse color of the light, green component.

\param db --- New diffuse color of the light, blue component.

\param da --- New diffuse color of the light, alpha component.

*/

void Light::setDiffuse(float dr, float dg, float db, float da)
{
    diffuse = glm::vec4(dr, dg, db, da);
}

/**
\brief Sets the specular color of the light.

\param sr --- New specular color of the light, red component.

\param sg --- New specular color of the light, green component.

\param sb --- New specular color of the light, blue component.

\param sa --- New specular color of the light, alpha component.

*/

void Light::setSpecular(float sr, float sg, float sb, float sa)
{
    specular = glm::vec4(sr, sg, sb, sa);
}

/**
\brief Sets the attenuation vector of the light.

\param attenc --- New attenuation vector of the light, constant component.

\param attenl --- New attenuation vector of the light, linear component.

\param attenq --- New attenuation vector of the light, quadratic component.

*/

void Light::setAttenuation(float attenc, float attenl, float attenq)
{
    attenuation = glm::vec3(attenc, attenl, attenq);
}
