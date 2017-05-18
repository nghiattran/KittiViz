#version 330 core

/**
\file VertexShaderEnvMap.glsl

\brief Vertex shader specifically for an environment map.  Incorporates
the transformation of vertices by a projection*view*model matrix. Calculates
the transformed position and normal vectors to be passes to the fragment shader.

\param [in] vposition --- vec4 vertex position from memory.

\param [in] vnormal --- vec4 normal vector from memory.

\param [out] normal --- vec3 transformed normal vector.

\param [out] position --- vec3 transformed vertex.

\param [uniform] PVM --- mat4 transformation matrix in the form projection*view*model.

\param [uniform] model --- mat4 model matrix.

*/

layout(location = 0) in vec4 vposition;
layout(location = 2) in vec3 vnormal;

uniform mat4 PVM;
uniform mat4 model;

out vec3 normal;
out vec3 position;

void main()
{
    gl_Position = PVM * vposition;
    normal = mat3(model) * vnormal;
    position =(model * vposition).xyz;
}
