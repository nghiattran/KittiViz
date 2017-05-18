#version 330 core

/**
\file VertexShaderBasicTexture.glsl

\brief Vertex shader that incorporates the transformation of vertices
by a projection*view*model matrix. Also, passes color and texture coordinate
information on to the fragment shader.

\param [in] vposition --- vec4 vertex position from memory.

\param [in] vcolor --- vec4 vertex color from memory.

\param [in] vnormal --- vec3 normal vector from memory.

\param [in] in_tex_coord --- vec2 texture coordinate from memory.

\param [out] color --- vec4 output color to the fragment shader.

\param [out] tex_coord --- vec2 texture coordinate to the fragment shader.

\param [uniform] PVM --- mat4 transformation matrix in the form projection*view*model.

*/

layout(location = 0) in vec4 vposition;
layout(location = 1) in vec4 vcolor;
layout(location = 2) in vec3 vnormal;      // Not needed in this example.
layout(location = 3) in vec2 in_tex_coord;

uniform mat4 PVM;

out vec4 color;
out vec2 tex_coord;

void main()
{
    tex_coord = in_tex_coord;
    color = vcolor;
    gl_Position = PVM * vposition;
}
