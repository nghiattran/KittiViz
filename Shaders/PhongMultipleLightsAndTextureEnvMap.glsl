#version 330 core

/**
\file PhongMultipleLightsAndTextureEnvMap.glsl

\brief Fragment shader that calculates Phong lighting for each fragment,
textual for the same fragment using multiple textures along with an
environmental map and combines all of them.

\param [in] position --- vec4 vertex position from memory.

\param [in] color --- vec4 vertex color from memory.

\param [in] normal --- vec3 normal vector from memory.

\param [in] tex_coord --- vec2 texture coordinate from memory.

\param [in] emnormal --- vec3 normal vector at the vertex for the environment map.

\param [in] emposition --- vec3 transformed position of the vertex, prior to
the view and projection transformations, for the environment map.

\param [out] fColor --- vec4 output color to the frame buffer.

\param [uniform] Lt --- Light struct containing a single light attribute set.

\param [uniform] Mat --- Material struct containing a single material attribute set.

\param [uniform] eye --- vec3 position of the viewer/camera.

\param [uniform] GlobalAmbient --- vec4 global ambient color vector.

\param [uniform] useTexture --- boolean array that determines if the texture is used.

\param [uniform] useReflection --- boolean that determines if the reflection is used.

\param [uniform] textrans --- mat4 texture transformation.

\param [uniform] tex --- sampler2D array, the texture.

\param [uniform] cmtex --- samplerCube, the cube map texture of the environment.

*/


struct Light
{
    bool on;             ///< Light on or off.
    vec4 position;       ///< Position of the light.
    vec3 spotDirection;  ///< Direction of the spot light.
    vec4 ambient;        ///< Ambient color of the light.
    vec4 diffuse;        ///< Diffuse color of the light.
    vec4 specular;       ///< Specular color of the light.
    float spotCutoff;    ///< Spot cutoff angle.
    float spotExponent;  ///< Spot falloff exponent.
    vec3 attenuation;    ///< Attenuation vector, x = constant, y = linear, z = quadratic.
};

struct Material
{
    vec4 ambient;     ///< Ambient color of the material.
    vec4 diffuse;     ///< Diffuse color of the material.
    vec4 specular;    ///< Specular color of the material.
    vec4 emission;    ///< Emission color of the material.
    float shininess;  ///< Shininess exponent of the material.
};

in vec4 position;
in vec4 color;
in vec3 normal;
in vec2 tex_coord;
in vec3 emnormal;
in vec3 emposition;

uniform Light Lt[10];
uniform Material Mat;
uniform vec3 eye;
uniform vec4 GlobalAmbient;
uniform int numLights;
uniform bool useTexture[10];
uniform mat4 textrans;
uniform bool useReflection;

uniform samplerCube cmtex;
uniform sampler2D tex[10];

out vec4 fColor;

void main()
{
    float deg = 0.017453292519943296;

    vec4 cc = vec4(0.0);
    bool usingLights = false;
    vec4 globalAmbientPortion = Mat.ambient*GlobalAmbient;

    for (int i = 0; i < numLights; i++)
    {
        if (Lt[i].on)
        {
            usingLights = true;
            vec3 n = normalize(normal);
            vec3 l = normalize(vec3(Lt[i].position)-vec3(position));
            vec3 r = normalize(2.0*dot(l,n)*n - l);
            vec3 v = normalize(eye-vec3(position));
            float lightDistance =length(vec3(Lt[i].position)-vec3(position));

            float dfang = max(0.0, dot(l, n));
            float specang = max(0.0, dot(r, v));
            if (dfang == 0)
                specang = 0;

            float attenuation = 1.0 / (Lt[i].attenuation[0] +
                                       Lt[i].attenuation[1] * lightDistance +
                                       Lt[i].attenuation[2] * lightDistance * lightDistance);

            float spotCos = dot(l, -normalize(Lt[i].spotDirection));
            float SpotCosCutoff = cos(Lt[i].spotCutoff*deg);  // assumes that spotCutoff is in degrees

            float spotFactor = 1.0;
            if (spotCos < SpotCosCutoff && Lt[i].spotCutoff < 179.9)  // Only fade if a spotlight
            {
                float range = 1 + SpotCosCutoff;
                spotFactor = pow(1 - (SpotCosCutoff - spotCos)/range, Lt[i].spotExponent);
            }

            vec4 ambientPortion = Mat.ambient*Lt[i].ambient;
            vec4 diffusePortion = Mat.diffuse*Lt[i].diffuse*dfang*attenuation*spotFactor;
            vec4 specularPortion = Mat.specular*Lt[i].specular*pow(specang, Mat.shininess)*attenuation*spotFactor;

            vec4 c = ambientPortion + diffusePortion + specularPortion;
            cc += min(c, vec4(1.0));
        }
    }

    cc = min(cc + globalAmbientPortion + Mat.emission, vec4(1.0));

    if (usingLights)
        fColor = cc;
    else
        fColor = color;

    vec4 texhom = vec4(tex_coord, 0, 1);
    vec4 transtex = textrans * texhom;
    vec2 transtex2 = vec2(transtex);

    int textureCount = 0;
    for (int i = 0; i < 10; i++)
        if (useTexture[i])
            textureCount++;

    vec4 texColor = vec4(0.0);
    for (int i = 0; i < 10; i++)
        if (useTexture[i])
            texColor += 1.0/textureCount * texture(tex[i], transtex2);

    if (textureCount > 0)
        fColor = 0.25*fColor + 0.75*texColor;

    fColor = min(fColor, vec4(1.0));

    if (useReflection)
    {
        vec3 tc = reflect(-eye + emposition, normalize(emnormal));
        fColor = min(0.75 * fColor + 0.25 * texture(cmtex, tc), vec4(1.0));
    }

}
