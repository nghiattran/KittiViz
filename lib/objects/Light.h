#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/**
\file Light.h
\brief Header file for Light.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class Light

\brief The Light class is simply a class for holding Phong model light properties.

*/

class Light {
private:
    bool on;                   ///< Light on or off.
    glm::vec4 position;        ///< Position of the light.
    glm::vec3 spotDirection;   ///< Direction of the spot light.
    glm::vec4 ambient;         ///< Ambient color of the light.
    glm::vec4 diffuse;         ///< Diffuse color of the light.
    glm::vec4 specular;        ///< Specular color of the light.
    float spotCutoff;          ///< Spot cutoff angle.
    float spotExponent;        ///< Spot falloff exponent.
    glm::vec3 attenuation;     ///< Attenuation vector, x = constant, y = linear, z = quadratic.

public:
    Light();

    Light(bool onoff,
          glm::vec4 pos,
          glm::vec3 spotDir,
          glm::vec4 a,
          glm::vec4 d,
          glm::vec4 s,
          float spotCut,
          float spotExp,
          glm::vec3 atten
         );

    Light(bool onoff,
          float posx, float posy, float posz, float posw,
          float spotDirx, float spotDiry, float spotDirz,
          float ar, float ag, float ab, float aa,
          float dr, float dg, float db, float da,
          float sr, float sg, float sb, float sa,
          float spotCut,
          float spotExp,
          float attenc, float attenl, float attenq
         );

    ~Light();

    bool getOn();
    glm::vec4 getPosition();
    glm::vec3 getSpotDirection();
    glm::vec4 getAmbient();
    glm::vec4 getDiffuse();
    glm::vec4 getSpecular();
    float getSpotCutoff();
    float getSpotExponent();
    glm::vec3 getAttenuation();

    void setOn(bool b);
    void setPosition(glm::vec4 p);
    void setSpotDirection(glm::vec3 sd);
    void setAmbient(glm::vec4 a);
    void setDiffuse(glm::vec4 d);
    void setSpecular(glm::vec4 s);
    void setSpotCutoff(float sc);
    void setSpotExponent(float se);
    void setAttenuation(glm::vec3 atten);

    void setPosition(float px, float py, float pz, float pw);
    void setSpotDirection(float sdx, float sdy, float sdz);
    void setAmbient(float ar, float ag, float ab, float aa);
    void setDiffuse(float dr, float dg, float db, float da);
    void setSpecular(float sr, float sg, float sb, float sa);
    void setAttenuation(float attenc, float attenl, float attenq);

    void setLight(bool onoff,
                  glm::vec4 pos,
                  glm::vec3 spotDir,
                  glm::vec4 a,
                  glm::vec4 d,
                  glm::vec4 s,
                  float spotCut,
                  float spotExp,
                  glm::vec3 atten
                 );

    void setLight(bool onoff,
                  float posx, float posy, float posz, float posw,
                  float spotDirx, float spotDiry, float spotDirz,
                  float ar, float ag, float ab, float aa,
                  float dr, float dg, float db, float da,
                  float sr, float sg, float sb, float sa,
                  float spotCut,
                  float spotExp,
                  float attenc, float attenl, float attenq
                 );
};

#endif // LIGHT_H_INCLUDED
