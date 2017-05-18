#version 330 core

/**
\file PassThroughFrag.glsl

\brief Simple pass through fragment shader.

This is a simple GLSL pass through fragment shader.

\param [in] color --- vec4 color from vertex shader.

\param [out] fColor --- vec4 output color to the frame buffer.

*/

in  vec4 color;
out vec4 fColor;

void main()
{
    fColor = color;
}
