#version 330 core

/**
\file VertexShaderCubeMap.glsl

\brief Vertex shader specifically for a cub map texture.  Incorporates
the transformation of vertices by a projection*view*model matrix. Sets
the texture coordinate to the position of the vertex.

\param [in] vposition --- vec4 vertex position from memory.

\param [out] tex_coord --- vec3 pass through of the texture coordinates.

\param [uniform] PVM --- mat4 transformation matrix in the form projection*view*model.

*/

layout(location = 0) in vec4 vposition;

uniform mat4 PVM;

out vec3 tex_coord;

void main()
{
    tex_coord = vec3(vposition);
    gl_Position = PVM * vposition;
}
