#version 400 core

/**
\file VertexShaderBasic3D.glsl

\brief Vertex shader that incorporates the transformation of vertices
by a projection*view*model matrix.

\param [in] position --- vec4 vertex position from memory.

\param [in] icolor --- vec4 vertex color from memory.

\param [out] color --- vec4 output color to the fragment shader.

\param [uniform] PVM --- mat4 transformation matrix in the form projection*view*model.

*/

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 icolor;

uniform mat4 PVM;

out vec4 color;

void main()
{
    color = icolor;
    gl_Position = PVM * position;
}
