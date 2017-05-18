#include <cstdlib>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "LoadShaders.h"

/**
\file LoadShaders.cpp
\brief Shader loader helper functions.

These functions were added to and altered from code found on the web.  There
was no reference to the original creator of the code in either the code files
or the web page linking to them.

\author    Don Spickler
\version   1.1
\date      Written: 11/21/2015  <BR>
Revised: 2/7/2016

\section use Usage

None of these functions will invoke the glUseProgram function since it the user may
wish to load in several shader programs.

\subsection loadvffile Loading Vertex and Fragment Shaders from Files

The files Shader1.vert and Shader1.frag are the vertex and fragment shaders respectively.

~~~~~~~~~~~~~~~{.c}
GLuint program = LoadShadersFromFile("Shader1.vert", "Shader1.frag");

if (!program)
{
    cerr << "Could not load Shader programs." << endl;
    exit(EXIT_FAILURE);
}

glUseProgram(program);
~~~~~~~~~~~~~~~

\subsection loadvfmemory Loading Vertex and Fragment Shaders from Memory

~~~~~~~~~~~~~~~{.c}
string vertexShader =
    "#version 330 core\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec4 icolor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "    color = icolor;\n"
    "    gl_Position = position;\n"
    "}\n";

string fragmentShader =
    "#version 330 core\n"
    "in  vec4 color;\n"
    "out vec4 fColor;\n"
    "void main()\n"
    "{\n"
    "    fColor = color;\n"
    "}\n";

GLuint program = LoadShadersFromMemory(vertexShader, fragmentShader);

if (!program)
{
    cerr << "Could not load Shader programs." << endl;
    exit(EXIT_FAILURE);
}

glUseProgram(program);
~~~~~~~~~~~~~~~

\subsection loadfile Loading Shaders from Files

The files Shader1.vert and Shader1.frag are the vertex and fragment shaders respectively.

~~~~~~~~~~~~~~~{.c}
ShaderInfo shaders[] =
{
    {GL_VERTEX_SHADER, "Shader1.vert"},
    {GL_FRAGMENT_SHADER, "Shader1.frag"},
    {GL_NONE}
};

GLuint program = LoadShadersFromFile(shaders);

if (!program)
{
    cerr << "Could not load Shader programs." << endl;
    exit(EXIT_FAILURE);
}

glUseProgram(program);
~~~~~~~~~~~~~~~

\subsection loadmemory Loading Shaders from Memory


~~~~~~~~~~~~~~~{.c}
string vertexShader =
    "#version 330 core\n"
    "layout(location = 0) in vec4 position;\n"
    "layout(location = 1) in vec4 icolor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "    color = icolor;\n"
    "    gl_Position = position;\n"
    "}\n";

string fragmentShader =
    "#version 330 core\n"
    "in  vec4 color;\n"
    "out vec4 fColor;\n"
    "void main()\n"
    "{\n"
    "    fColor = color;\n"
    "}\n";

ShaderInfo shaders[] =
{
    {GL_VERTEX_SHADER, "", vertexShader},
    {GL_FRAGMENT_SHADER, "", fragmentShader},
    {GL_NONE}
};

GLuint program = LoadShadersFromMemory(shaders);

if (!program)
{
    cerr << "Could not load Shader programs." << endl;
    exit(EXIT_FAILURE);
}

glUseProgram(program);
~~~~~~~~~~~~~~~

---

\copyright GNU Public License.

This software is provided as-is, without warranty of ANY KIND, either expressed or implied,
including but not limited to the implied warranties of merchant ability and/or fitness for a
particular purpose. The authors shall NOT be held liable for ANY damage to you, your computer,
or to anyone or anything else, that may result from its use, or misuse.
All trademarks and other registered names contained in this package are the property
of their respective owners.  USE OF THIS SOFTWARE INDICATES THAT YOU AGREE TO THE ABOVE CONDITIONS.

---

*/

/**

\brief Reads a shader file and returns the string of the file contents.
Used by the LoadShadersFromFile functions.

\param filename --- Name of shader file to be loaded.

\return String of the file contents.

*/

const GLchar* ReadShader(const char* filename)
{
    FILE* infile = fopen(filename, "rb");

    if (!infile)
    {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek(infile, 0, SEEK_END);
    int len = ftell(infile);
    fseek(infile, 0, SEEK_SET);

    GLchar* source = new GLchar[len+1];

    fread(source, 1, len, infile);
    fclose(infile);

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

/**

\brief Takes a ShaderInfo array containing the shader types and filenames for the
shader code.

Reads the files, compiles and links the code, and returns the
shader program identifier, or 0 on failure.  The array of structures is terminated
by a final Shader with the "type" field set to GL_NONE.

\param shaders --- An array of ShaderInfo structures, one for each type of
shader to be loaded into a shader program.

\return Identifier for the shader program.

*/

GLuint LoadShadersFromFile(ShaderInfo* shaders)
{
    if (shaders == NULL)
        return 0;

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while (entry->type != GL_NONE)
    {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const GLchar* source = ReadShader(entry->filename);
        if (source == NULL)
        {
            for (entry = shaders; entry->type != GL_NONE; ++entry)
            {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource(shader, 1, &source, NULL);
        delete [] source;

        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */

            return 0;
        }

        glAttachShader(program, shader);
        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog(program, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete [] log;
#endif /* DEBUG */

        for (entry = shaders; entry->type != GL_NONE; ++entry)
        {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return 0;
    }

    return program;
}

/**

\brief Takes a ShaderInfo array containing the shader types and strings of the
shader code.

Compiles and links the code, and returns the
shader program identifier, or 0 on failure.  The array of structures is terminated
by a final Shader with the "type" field set to GL_NONE.

\param shaders --- An array of ShaderInfo structures, one for each type of
shader to be loaded into a shader program.

\return Identifier for the shader program.

*/

GLuint LoadShadersFromMemory(ShaderInfo* shaders)
{
    if (shaders == NULL)
        return 0;

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while (entry->type != GL_NONE)
    {
        GLuint shader = glCreateShader(entry->type);

        entry->shader = shader;

        const GLchar* source = (GLchar*)(entry->code.c_str());
        if (source == NULL)
        {
            for (entry = shaders; entry->type != GL_NONE; ++entry)
            {
                glDeleteShader(entry->shader);
                entry->shader = 0;
            }

            return 0;
        }

        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */

            return 0;
        }

        glAttachShader(program, shader);
        ++entry;
    }

    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog(program, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete [] log;
#endif /* DEBUG */

        for (entry = shaders; entry->type != GL_NONE; ++entry)
        {
            glDeleteShader(entry->shader);
            entry->shader = 0;
        }

        return 0;
    }

    return program;
}

/**

\brief Takes the filenames for the vertex shader and the fragment shader and loads
them into a shader program.

Reads the files, compiles and links the code, and returns the
shader program identifier, or 0 on failure.  The array of structures is terminated
by a final Shader with the "type" field set to GL_NONE.

\param vertfn --- The filename for the vertex shader.

\param fragfn --- The filename for the fragment shader.

\return Identifier for the shader program.

*/

GLuint LoadShadersFromFile(std::string vertfn, std::string fragfn)
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, vertfn.c_str()},
        {GL_FRAGMENT_SHADER, fragfn.c_str()},
        {GL_NONE}
    };

    return LoadShadersFromFile(shaders);
}

/**

\brief Takes the code strings for the vertex shader and the fragment shader and loads
them into a shader program.

Compiles and links the code, and returns the
shader program identifier, or 0 on failure.  The array of structures is terminated
by a final Shader with the "type" field set to GL_NONE.

\param vertcode --- The code string for the vertex shader.

\param fragcode --- The code string for the fragment shader.

\return Identifier for the shader program.

*/

GLuint LoadShadersFromMemory(std::string vertcode, std::string fragcode)
{
    ShaderInfo shaders[] =
    {
        {GL_VERTEX_SHADER, "", vertcode},
        {GL_FRAGMENT_SHADER, "", fragcode},
        {GL_NONE}
    };

    return LoadShadersFromMemory(shaders);
}
