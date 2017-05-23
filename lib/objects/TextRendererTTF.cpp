#include "TextRendererTTF.h"


/**
\file TextRendererTTF.cpp
\brief This class encapsulates the details of rendering a TrueType font on an OpenGL
window.

The majority of the code in this class structure was taken from
OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
The original file was in the public domain and was written by Guus Sliepen and
Sylvain Beucler.  It was then altered by Don Spickler to remove its dependency
on SDL and make it more transferable between windowing systems. In addition,
the code was written to be easy to integrate into the modern OpenGL graphics
rendering system and rotation of text was added to the functionality.

\author    Guus Sliepen, Sylvain Beucler, and Don Spickler
\version   1.1
\date      Written: 1/15/2016  <BR> Revised: 1/15/2016

*/

/**
\brief Constructor

\param fontFile --- Optional, the filename, and relative path, of the TruType font file to be loaded.

Creates a text renderer object,

- Loads the default attributes
- Initializes FreeType
- Initializes GLEW
- Loads the text rendering shaders (stored in memory)
- Generates the needed buffers
- Calls the font loading method if a font file was given in the parameter list.

*/

TextRendererTTF::TextRendererTTF(std::string fontFile)
{
    color[0] = 1;
    color[1] = 1;
    color[2] = 1;
    color[3] = 1;
    TextVBO = -1;
    TextVAO = -1;
    fontSize = 16;

    fontLoadError = GL_FALSE;
    shaderError = GL_FALSE;
    freetypeError = GL_FALSE;
    glewError = GL_FALSE;

    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "Could not initialize freetype library." << std::endl;
        freetypeError = GL_TRUE;
    }

    // Turn on GLEW
    if (glewInit())
    {
        std::cerr << "Unable to initialize GLEW." << std::endl;
        glewError = GL_TRUE;
    }

    std::string vertShade =
        "#version 330\n"
        "attribute vec4 coord;\n"
        "varying vec2 texpos;\n"
        "uniform mat4 rotMat;\n"
        "void main(void) {\n"
        "  gl_Position = rotMat * vec4(coord.xy, 0, 1);\n"
        "  texpos = coord.zw;\n"
        "}\n";

    std::string fragShade =
        "#version 330\n"
        "varying vec2 texpos;\n"
        "uniform sampler2D tex;\n"
        "uniform vec4 color;\n"
        "void main(void) {\n"
        "  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;\n"
        "}\n";

    program = LoadShadersFromMemory(vertShade, fragShade);

    if(program == 0)
    {
        std::cerr << "Unable to create text shader." << std::endl;
        shaderError = GL_TRUE;
    }

    attribute_coord = glGetAttribLocation(program, "coord");
    uniform_tex = glGetUniformLocation(program, "tex");
    uniform_color = glGetUniformLocation(program, "color");
    rotmat = glGetUniformLocation(program, "rotMat");

    if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1 || rotmat == -1)
    {
        std::cerr << "Unable to link attributes." << std::endl;
        shaderError = GL_TRUE;
    }

    // Create the vertex buffer object
    glGenBuffers(1, &TextVBO);
    glGenVertexArrays(1, &TextVAO);

    if (fontFile.compare("") != 0)
        loadFont(fontFile);
}


/**
\brief Destructor

No implementation needed at this time.

*/

TextRendererTTF::~TextRendererTTF() {}

/**
\brief Reports font, shader, freetype, and glew loading and initializing
errors.

Tracks any errors and reports a boolean to the calling method.
Returns false if all is well and text can be rendered to the screen.
Returns true if there is an error that would prevent text rendering.

*/

GLboolean TextRendererTTF::isError()
{
    if (fontLoadError || shaderError || freetypeError || glewError)
    {
        if (fontLoadError)
            std::cerr << "Font Load Error" << std::endl;

        if (shaderError)
            std::cerr << "Shader Error" << std::endl;

        if (freetypeError)
            std::cerr << "Freetype Error" << std::endl;

        if (glewError)
            std::cerr << "GLEW Error" << std::endl;

        return GL_TRUE;
    }
    else
        return GL_FALSE;
}

/**
\brief Sets the size of the font in pixels.

\param sz --- Size of the font.

*/

void TextRendererTTF::setFontSize(GLuint sz)
{
    fontSize = sz;
}

/**
\brief Returns the current font size, that is, the number of vertical pixels used in
the diaplay of the font.

*/

GLuint TextRendererTTF::getFontSize()
{
    return fontSize;
}

/**
\brief Sets the color of the font.

Font colors are given in (r, g, b, a) coordinates, values are in [0, 1] with
0 being no intensity (and transparent for the alpha channel) to 1 being
the highest intensity (and opaque for the alpha channel)

\param r --- Red

\param g --- Green

\param b --- Blue

\param a --- Alpha

*/

void TextRendererTTF::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}

/**
\brief Sets the color of the font.

Font colors are given in (r, g, b, a) coordinates, values are in [0, 1] with
0 being no intensity (and transparent for the alpha channel) to 1 being
the highest intensity (and opaque for the alpha channel)

\param c --- Vector of 4 floats representing (r, g, b, a) values.

*/

void TextRendererTTF::setColor(GLfloat c[4])
{
    color[0] = c[0];
    color[1] = c[1];
    color[2] = c[2];
    color[3] = c[3];
}

/**
\brief Loads a TrueType font file into the rendering system.

\param fontFile --- String file name and path to the font file.

*/

void TextRendererTTF::loadFont(std::string fontFile)
{
    fontLoadError = GL_FALSE;
    FILE* infile = fopen(fontFile.c_str(), "rb");

    if (!infile)
    {
        std::cerr << "Could not find font file " << fontFile << std::endl;
        fontLoadError = GL_TRUE;
    }
    else
    {
        fseek(infile, 0, SEEK_END);
        int fontsize = ftell(infile);
        fseek(infile, 0, SEEK_SET);
        GLchar* source = new GLchar[fontsize+1];
        fread(source, 1, fontsize, infile);
        fclose(infile);
        source[fontsize] = 0;
        char* font = const_cast<char*>(source);

        if (font == NULL)
        {
            std::cerr << "Could not load font file " << fontFile << std::endl;
            fontLoadError = GL_TRUE;
        }

        FT_Error fterr = FT_New_Memory_Face(ft, (FT_Byte*)font, fontsize, 0, &face);

        if (fterr != FT_Err_Ok)
        {
            std::cerr << "Could not initialize font." << std::endl;
            freetypeError = GL_TRUE;
        }
    }
}

/**
\brief Renders text to the OpenGL window.

\param text --- String to be displayed..

*/

void TextRendererTTF::draw(std::string text)
{
    draw(text.c_str());
}

/**
\brief Renders text to the OpenGL window.

\param text --- char* type string to be displayed.

*/

void TextRendererTTF::draw(const char* text)
{
    if (isError())
        return;

    glUseProgram(program);
    glBindVertexArray(TextVAO);

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glUniform4fv(uniform_color, 1, color);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const char *p;
    FT_GlyphSlot g = face->glyph;
    GLfloat x = 0;
    GLfloat y = 0;

    /* Create a texture that will be used to hold one "glyph" */
    GLuint tex;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(uniform_tex, 0);

    /* We require 1 byte alignment when uploading texture data */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* Clamping to edges is important to prevent artifacts when scaling */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Linear filtering usually looks best for text */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Set up the VBO for our vertex data */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
    glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    /* Loop through all characters */
    for (p = text; *p; p++)
    {
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        /* Calculate the vertex and texture coordinates */
        GLfloat x2 = x + g->bitmap_left;
        GLfloat y2 = -y - g->bitmap_top;
        GLfloat w = g->bitmap.width;
        GLfloat h = g->bitmap.rows;

        TextRendererTTFPoint box[4] =
        {
            {x2, -y2, 0, 0},
            {x2 + w, -y2, 1, 0},
            {x2, -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        /* Draw the character on the screen */
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        /* Advance the cursor to the start of the next character */
        x += g->advance.x >> 6;
        y += g->advance.y >> 6;
    }

    glDisableVertexAttribArray(attribute_coord);
    glDeleteTextures(1, &tex);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

/**
\brief Returns the pixel width of the text using the current font and size.

\param text --- String to be analyzed.

*/

int TextRendererTTF::textWidth(std::string text)
{
    return textWidth(text.c_str());
}

/**
\brief Returns the pixel width of the text using the current font and size.

\param text --- char* type string to be analyzed.

*/

int TextRendererTTF::textWidth(const char* text)
{
    if (isError())
        return 0;

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    FT_GlyphSlot g = face->glyph;
    const char *p;
    int  x = 0;

    for (p = text; *p; p++)
    {
        /* Try to load and render the character */
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        /* Advance the cursor to the start of the next character */
        x += g->advance.x >> 6;
    }

    return x;
}


void TextRendererTTF::setModelMatrix(glm::mat4 model)
{
    glUseProgram(program);
    glUniformMatrix4fv(rotmat, 1, GL_FALSE, glm::value_ptr(model));
}