#include "ObjModel.h"

/**
\file ObjModel.cpp
\brief Implementation for graphing Wavefront obj files.

\author    Don Spickler
\author    Nghia Tran
\version   1.2
\date      Written: 4/10/2016  <BR> Revised: 3/20/2017

*/

/**
\brief Constructor

Default constructor.

*/

ObjModel::ObjModel()
{
    numLights = 0;
    GlobalAmbient = glm::vec4(0);

    VBOs.clear();
    MatNames.clear();
    VertexSizes.clear();
    vertices.clear();
    texcoords.clear();
    normals.clear();
    mats.clear();
    textures.clear();
    programs.clear();

    mat.setMaterial(0,0,0,1,0.6,0.6,0.6,1,0.6,0.6,0.6,1,0,0,0,1,0);
}

/**
\brief Destructor

Removes the stored data.

*/

ObjModel::~ObjModel()
{
    VBOs.clear();
    MatNames.clear();
    VertexSizes.clear();
    vertices.clear();
    texcoords.clear();
    normals.clear();
    mats.clear();
    textures.clear();
    programs.clear();
}

/**
\brief Loads the data from the vertex, normal, and texture coordinate
vectors to the graphics card.

\param matname --- name of the material that is to be used on the data
that is being loaded.

*/

void ObjModel::LoadDataToGraphicsCard(std::string matname)
{
    GLint vPosition = 0;
    GLint vNormal = 2;
    GLint vTex = 3;

    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &bufptr);

    VBOs.push_back(vboptr);
    MatNames.push_back(matname);
    VertexSizes.push_back(vertices.size());

    unsigned int vsize = vertices.size()*sizeof(glm::vec3);
    unsigned int nsize = normals.size()*sizeof(glm::vec3);
    unsigned int tsize = texcoords.size()*sizeof(glm::vec2);

    glBindVertexArray(vboptr);
    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, vsize + nsize + tsize, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vsize, nsize, &normals[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vsize + nsize, tsize, &texcoords[0]);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vsize));
    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vsize + nsize));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);
}

/**
\brief Removes the path portion of a filename.

\param filename --- filename to remove path.

*/

std::string ObjModel::RemovePath(std::string filename)
{
    std::string retstr;
    retstr = filename;

    int pos1 = filename.find_last_of("\\");
    int pos2 = filename.find_last_of("/");

    if (pos1 != -1 || pos2 != -1)
    {
        int pos = pos1;
        if (pos2 > pos)
            pos = pos2;

        retstr = retstr.substr(pos + 1);
    }

    return retstr;
}

/**
\brief Loads in the texture images that are referenced in the materials
file.

\param path --- path from the program to the folder containing the images.

*/

bool ObjModel::LoadTextures(std::string path)
{
    objTexture objtex;

    for(unsigned int i = 0; i < mats.size(); i++)
    {
        if (!mats[i].ATextureFile.empty())
        {
            bool found = false;
            for (unsigned int j = 0; j < textures.size(); j++)
                if (textures[j].Filename.compare(mats[i].ATextureFile) == 0)
                    found = true;

            if (!found)
            {
                objtex.Filename = mats[i].ATextureFile;
                if (objtex.Texture.loadFromFile(path + objtex.Filename))
                    textures.push_back(objtex);
                else
                    std::cerr << "Could not load texture: " << objtex.Filename << std::endl;
            }
        }

        if (!mats[i].DTextureFile.empty())
        {
            bool found = false;
            for (unsigned int j = 0; j < textures.size(); j++)
                if (textures[j].Filename.compare(mats[i].DTextureFile) == 0)
                    found = true;

            if (!found)
            {
                objtex.Filename = mats[i].DTextureFile;
                if (objtex.Texture.loadFromFile(path + objtex.Filename))
                    textures.push_back(objtex);
                else
                    std::cerr << "Could not load texture: " << objtex.Filename << std::endl;
            }
        }

        if (!mats[i].STextureFile.empty())
        {
            bool found = false;
            for (unsigned int j = 0; j < textures.size(); j++)
                if (textures[j].Filename.compare(mats[i].STextureFile) == 0)
                    found = true;

            if (!found)
            {
                objtex.Filename = mats[i].STextureFile;
                if (objtex.Texture.loadFromFile(path + objtex.Filename))
                    textures.push_back(objtex);
                else
                    std::cerr << "Could not load texture: " << objtex.Filename << std::endl;
            }
        }
    }

    return true;
}

/**
\brief Loads the set of materials in the materials file that is referenced
from the Wavefromt obj file.  At the end the method the texture loader is
called to load in the texture images that are referenced in the materials
file.

\param path --- path from the program to the folder containing the data.

\param filename --- name of the obj file containing the data.

*/

bool ObjModel::LoadMateials(std::string path, std::string filename)
{
    std::string fullFilename = path + filename;
    FILE * file = fopen(fullFilename.c_str(), "r");

    objMaterial objmat;
    objmat.name = "";
    objmat.mat.setMaterial(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0);
    objmat.ATextureFile = "";
    objmat.DTextureFile = "";
    objmat.STextureFile = "";

    if(file == NULL)
    {
        std::cout << "Cannot open file: " << filename << std::endl;
        return false;
    }

    bool finished = false;
    bool firstmat = true;

    while(!finished)
    {
        char lineHeader[512];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            finished = true;
            mats.push_back(objmat);
        }
        else
        {
            if (strcmp(lineHeader, "newmtl") == 0)
            {
                char MaterialName[4096];
                fscanf(file, "%s", MaterialName);

                if (firstmat)
                {
                    objmat.name = MaterialName;
                    firstmat = false;
                }
                else
                {
                    mats.push_back(objmat);
                    objmat.name = MaterialName;
                    objmat.mat.setMaterial(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0);
                    objmat.ATextureFile = "";
                    objmat.DTextureFile = "";
                    objmat.STextureFile = "";
                }
            }
            else if (strcmp(lineHeader, "Ns") == 0)
            {
                float sh = 0;
                int matches = fscanf(file, "%f\n", &sh);
                if (matches == 1)
                    objmat.mat.setShininess(sh);
            }
            else if (strcmp(lineHeader, "Ka") == 0)
            {
                glm::vec4 color;
                color.a = 1;
                int matches = fscanf(file, "%f %f %f\n", &color.r, &color.g, &color.b);
                if (matches == 3)
                    objmat.mat.setAmbient(color);
            }
            else if (strcmp(lineHeader, "Kd") == 0)
            {
                glm::vec4 color;
                color.a = 1;
                int matches = fscanf(file, "%f %f %f\n", &color.r, &color.g, &color.b);
                if (matches == 3)
                    objmat.mat.setDiffuse(color);
            }
            else if (strcmp(lineHeader, "Ks") == 0)
            {
                glm::vec4 color;
                color.a = 1;
                int matches = fscanf(file, "%f %f %f\n", &color.r, &color.g, &color.b);
                if (matches == 3)
                    objmat.mat.setSpecular(color);
            }
            else if (strcmp(lineHeader, "Ke") == 0)
            {
                glm::vec4 color;
                color.a = 1;
                int matches = fscanf(file, "%f %f %f\n", &color.r, &color.g, &color.b);
                if (matches == 3)
                    objmat.mat.setEmission(color);
            }
            else if (strcmp(lineHeader, "map_Ka") == 0)
            {
                char TextureName[4096];
                fscanf(file, "%s", TextureName);
                objmat.ATextureFile = RemovePath(TextureName);
            }
            else if (strcmp(lineHeader, "map_Kd") == 0)
            {
                char TextureName[4096];
                fscanf(file, "%s", TextureName);
                objmat.DTextureFile = RemovePath(TextureName);
            }
            else if (strcmp(lineHeader, "map_Ks") == 0)
            {
                char TextureName[4096];
                fscanf(file, "%s", TextureName);
                objmat.STextureFile = RemovePath(TextureName);
            }
        }
    }

    fclose(file);
    LoadTextures(path);
    return true;
}


/**
\brief Loads the data from a Wavefront obj file and stores the data in
the vertex, normal, and texture coordinate vectors.  Finally, the loader
method is called to load the data to the graphics card.  This is done for
each segment of the model.  In addition, the material file is read from the
obj file and the material loader is called to load in the material information.
This method also calls the texture loader to load the testures that are
referenced in the materials file.  At the end of the method, the program
loader is called to create a shader program for each material.

\param path --- path from the program to the folder containing the data.

\param filename --- name of the obj file containing the data.

*/

bool ObjModel::Load(std::string path, std::string filename)
{
    std::vector<unsigned int> vertexIndices, texIndices, normalIndices;
    std::vector<glm::vec3> file_vertices;
    std::vector<glm::vec2> file_texcoords;
    std::vector<glm::vec3> file_normals;

    std::string nextmatname = "";
    std::string matname = "";
    bool firstfaceset = true;

    std::string fullFilename = path + filename;

    FILE * file = fopen(fullFilename.c_str(), "r");

    if(file == NULL)
    {
        std::cout << "Cannot open file: " << filename << std::endl;
        return false;
    }

    vertices.clear();
    texcoords.clear();
    normals.clear();

    bool finished = false;
    while(!finished)
    {
        char lineHeader[512];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
        {
            finished = true;
        }
        else
        {
            if (strcmp(lineHeader, "mtllib") == 0)
            {
                char MaterialFilename[4096];
                fscanf(file, "%s", MaterialFilename);
                std::string matfile = MaterialFilename;

                LoadMateials(path, matfile);
            }
            else if (strcmp(lineHeader, "usemtl") == 0)
            {
                matname = nextmatname;
                char MaterialName[4096];
                fscanf(file, "%s", MaterialName);
                nextmatname = MaterialName;

                if (firstfaceset)
                {
                    firstfaceset = false;
                }
                else
                {
                    for(unsigned int i = 0; i < vertexIndices.size(); i++)
                    {
                        unsigned int vertexIndex = vertexIndices[i];
                        glm::vec3 vertex = file_vertices[vertexIndex - 1];
                        vertices.push_back(vertex);
                    }

                    for(unsigned int i = 0; i < normalIndices.size(); i++)
                    {
                        unsigned int normalIndex = normalIndices[i];
                        glm::vec3 normal = file_normals[normalIndex - 1];
                        normals.push_back(normal);
                    }

                    for(unsigned int i = 0; i < texIndices.size(); i++)
                    {
                        unsigned int texIndex = texIndices[i];
                        glm::vec2 texcoord = file_texcoords[texIndex - 1];
                        texcoords.push_back(texcoord);
                    }

                    LoadDataToGraphicsCard(matname);

                    vertexIndices.clear();
                    texIndices.clear();
                    normalIndices.clear();
                    vertices.clear();
                    texcoords.clear();
                    normals.clear();
                }
            }
            else if (strcmp(lineHeader, "v") == 0)
            {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                file_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                glm::vec2 tex;
                fscanf(file, "%f %f\n", &tex.x, &tex.y);
                file_texcoords.push_back(tex);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                file_normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

                vertexIndex[0] = 1;
                vertexIndex[1] = 1;
                vertexIndex[2] = 1;
                uvIndex[0] = 1;
                uvIndex[1] = 1;
                uvIndex[2] = 1;
                normalIndex[0] = 1;
                normalIndex[1] = 1;
                normalIndex[2] = 1;

                long filepos = ftell(file);
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches == 9)
                {
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    texIndices.push_back(uvIndex[0]);
                    texIndices.push_back(uvIndex[1]);
                    texIndices.push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }
                else
                {
                    fseek(file, filepos, SEEK_SET);
                    matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                    if (matches == 6)
                    {
                        vertexIndices.push_back(vertexIndex[0]);
                        vertexIndices.push_back(vertexIndex[1]);
                        vertexIndices.push_back(vertexIndex[2]);
                        normalIndices.push_back(normalIndex[0]);
                        normalIndices.push_back(normalIndex[1]);
                        normalIndices.push_back(normalIndex[2]);
                    }
                    else
                    {
                        fseek(file, filepos, SEEK_SET);
                        matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
                        if (matches == 6)
                        {
                            vertexIndices.push_back(vertexIndex[0]);
                            vertexIndices.push_back(vertexIndex[1]);
                            vertexIndices.push_back(vertexIndex[2]);
                            texIndices.push_back(uvIndex[0]);
                            texIndices.push_back(uvIndex[1]);
                            texIndices.push_back(uvIndex[2]);
                        }
                        else
                        {
                            fseek(file, filepos, SEEK_SET);
                            matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
                            if (matches == 3)
                            {
                                vertexIndices.push_back(vertexIndex[0]);
                                vertexIndices.push_back(vertexIndex[1]);
                                vertexIndices.push_back(vertexIndex[2]);
                            }
                            else
                            {
                                std::cout << "File read error" << std::endl;
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    for(unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = file_vertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < normalIndices.size(); i++)
    {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = file_normals[normalIndex - 1];
        normals.push_back(normal);
    }

    for(unsigned int i = 0; i < texIndices.size(); i++)
    {
        unsigned int texIndex = texIndices[i];
        glm::vec2 texcoord = file_texcoords[texIndex - 1];
        texcoords.push_back(texcoord);
    }

    matname = nextmatname;
    LoadDataToGraphicsCard(matname);

    vertexIndices.clear();
    texIndices.clear();
    normalIndices.clear();
    vertices.clear();
    texcoords.clear();
    normals.clear();

    fclose(file);

    LoadPrograms();

    return true;
}


/**
\brief Loads the GLSL programs to the graphics card.  There is one
shader program for each material in the Wavwefront model.  In
the draw method, the material is stored for each segment of the
model, matched to the correct shader program and the porgram
is used.

*/

bool ObjModel::LoadPrograms()
{
    for (unsigned int i = 0; i < mats.size(); i++)
    {
        GLuint program = LoadShadersFromFile("Shaders/ObjVertexShader.glsl", "Shaders/ObjFragmentShader.glsl");

        if (!program)
        {
            std::cerr << "Could not load Wavefront Object Shader programs." << std::endl;
            exit(EXIT_FAILURE);
        }

        programs.push_back(program);

        // Turn on the shader & get location of transformation matrix.
        glUseProgram(program);

        GLuint Atexloc = glGetUniformLocation(program, "useTexA");
        GLuint Dtexloc = glGetUniformLocation(program, "useTexD");
        GLuint Stexloc = glGetUniformLocation(program, "useTexS");

        objMaterial objmat = mats[i];
        loadMaterial(program, objmat.mat);

        glUniform1i(Atexloc, false);
        if (!objmat.ATextureFile.empty())
            for (unsigned int k = 0; k < textures.size(); k++)
                if (textures[k].Filename.compare(objmat.ATextureFile) == 0)
                {
                    loadTexture(program, textures[k].Texture, "texA");
                    glUniform1i(Atexloc, true);
                }

        glUniform1i(Dtexloc, false);
        if (!objmat.DTextureFile.empty())
            for (unsigned int k = 0; k < textures.size(); k++)
                if (textures[k].Filename.compare(objmat.DTextureFile) == 0)
                {
                    loadTexture(program, textures[k].Texture, "texD");
                    glUniform1i(Dtexloc, true);
                }

        glUniform1i(Stexloc, false);
        if (!objmat.STextureFile.empty())
            for (unsigned int k = 0; k < textures.size(); k++)
                if (textures[k].Filename.compare(objmat.STextureFile) == 0)
                {
                    loadTexture(program, textures[k].Texture, "texS");
                    glUniform1i(Stexloc, true);
                }
    }

    return true;
}


/**
\brief Draws the object.

*/

void ObjModel::draw()
{
    for (unsigned int i = 0; i < VBOs.size(); i++)
    {
        std::string matname = MatNames[i];

        int k = -1;

        for (unsigned int j = 0; j < mats.size(); j++)
            if (matname.compare(mats[j].name) == 0)
                k = j;

        if (k == -1)
            return;

        glUseProgram(programs[k]);
        glBindVertexArray(VBOs[i]);
        glDrawArrays(GL_TRIANGLES, 0, VertexSizes[i]);
    }
}

/**
\brief Draws the object given all information.

\param GLuint --- PVMLoc.

\param glm::mat4 --- projection matrix.
\param glm::mat4 --- view matrix.
\param glm::mat4 --- normal matrix.

*/


void ObjModel::draw(glm::mat4 p, glm::mat4 v) {
    setProjectionMatrix(p);
    setViewMatrix(v);
    draw();
}

/**
\brief Loads the material structure to the shader material structure.

\param program --- The address of the program to alter.

\param mat --- Material to load.

*/

void ObjModel::loadMaterial(GLuint program, Material mat)
{
    glUseProgram(program);

    glUniform4fv(glGetUniformLocation(program, "Mat.ambient"), 1, glm::value_ptr(mat.getAmbient()));
    glUniform4fv(glGetUniformLocation(program, "Mat.diffuse"), 1, glm::value_ptr(mat.getDiffuse()));
    glUniform4fv(glGetUniformLocation(program, "Mat.specular"), 1, glm::value_ptr(mat.getSpecular()));
    glUniform4fv(glGetUniformLocation(program, "Mat.emission"), 1, glm::value_ptr(mat.getEmission()));
    glUniform1f(glGetUniformLocation(program, "Mat.shininess"), mat.getShininess());
}


/**
\brief Loads a texture image to the graphics card.

\param program --- The address of the program to alter.

\param tex --- Image to load.

\param name --- The name of the texture in the shader.

*/

void ObjModel::loadTexture(GLuint program, sf::Image tex, std::string name)
{
    glUseProgram(program);

    GLuint TexID;
    glGenTextures(1, &TexID);

    //  Link the texture to the shader.
    GLuint tex_uniform_loc = glGetUniformLocation(program, name.c_str());
    glUniform1i(tex_uniform_loc, TexID);

    //  Load the texture into texture memory.
    glActiveTexture(GL_TEXTURE0+TexID);
    glBindTexture(GL_TEXTURE_2D, TexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.getSize().x, tex.getSize().y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, tex.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


/**
\brief Loads a single light into a light array in the shader.

\param program --- The address of the program to alter.

\param Lt --- Light to load.

\param name --- The name of the array in the shader.

\param i --- The index of the light to load.

*/

void ObjModel::LoadLight(GLuint program, Light Lt, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), Lt.getOn());

    sprintf(locID, "%s[%d].%s", arrayname, i, "position");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getPosition()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "ambient");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAmbient()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "diffuse");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getDiffuse()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "specular");
    glUniform4fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpecular()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotDirection");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getSpotDirection()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "attenuation");
    glUniform3fv(glGetUniformLocation(program, locID), 1, glm::value_ptr(Lt.getAttenuation()));

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotCutoff");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotCutoff());

    sprintf(locID, "%s[%d].%s", arrayname, i, "spotExponent");
    glUniform1f(glGetUniformLocation(program, locID), Lt.getSpotExponent());
}

/**
\brief Loads the first num entries of a light array to the a light array in the shader.

\param program --- The address of the program to alter.

\param Lt --- Light array to load.

\param name --- The name of the array in the shader.

\param i --- The number of lights to load.

*/

void ObjModel::LoadLights(GLuint program, Light Lt[], std::string name, int num)
{
    glUseProgram(program);

    for (int i = 0; i < num; i++)
        LoadLight(program, Lt[i], name.c_str(), i);
}

/**
\brief Turns the light at index i on.

\param program --- The address of the program to alter.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn on.

*/

void ObjModel::turnLightOn(GLuint program, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), true);
}

/**
\brief Turns the light at index i off.

\param program --- The address of the program to alter.

\param name --- The name of the array in the shader.

\param i --- The index of the light to turn off.

*/

void ObjModel::turnLightOff(GLuint program, std::string name, int i)
{
    glUseProgram(program);

    const char* arrayname = name.c_str();  // array name in the shader.
    char locID[100];
    sprintf(locID, "%s[%d].%s", arrayname, i, "on");
    glUniform1i(glGetUniformLocation(program, locID), false);
}

/**
\brief Sets the view matrix.

\param viewMatrix --- glm::mat4 that represents the view matrix.

*/

void ObjModel::setViewMatrix(glm::mat4 viewMatrix)
{
    view = viewMatrix;
    for (unsigned int i = 0; i < programs.size(); i++)
        setViewMatrix(programs[i], viewMatrix);
}

/**
\brief Sets the projection matrix.

\param projectionMatrix --- glm::mat4 that represents the projection matrix.

*/

void ObjModel::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    projection = projectionMatrix;
    for (unsigned int i = 0; i < programs.size(); i++)
        setProjectionMatrix(programs[i], projectionMatrix);
}

/**
\brief Sets the model matrix.

\param modelMatrix --- glm::mat4 that represents the model matrix.

*/

void ObjModel::setModelMatrix(glm::mat4 modelMatrix)
{
    model = modelMatrix;
    for (unsigned int i = 0; i < programs.size(); i++)
        setModelMatrix(programs[i], modelMatrix);
}

/**
\brief Sets the eye position.

\param Eye --- glm::vec3 holding the (x, y, z) position of the eye.

*/

void ObjModel::setEye(glm::vec3 Eye)
{
    eye = Eye;
    for (unsigned int i = 0; i < programs.size(); i++)
        setEye(programs[i], Eye);
}


/**
\brief Sets the view matrix to the specified program.

\param program --- program address for the program to alter.

\param viewMatrix --- glm::mat4 that represents the view matrix.

*/

void ObjModel::setViewMatrix(GLuint program, glm::mat4 viewMatrix)
{
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "PVM"),
        1, GL_FALSE, glm::value_ptr(projection*view*model));
}

/**
\brief Sets the projection matrix to the specified program.

\param program --- program address for the program to alter.

\param projectionMatrix --- glm::mat4 that represents the projection matrix.

*/

void ObjModel::setProjectionMatrix(GLuint program, glm::mat4 projectionMatrix)
{
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "PVM"),
        1, GL_FALSE, glm::value_ptr(projection*view*model));
}

/**
\brief Sets the object model matrix.  Also calculates and loads the PVM matrix
and normal matrices.  Applies the setting to the specified program.

\param program --- program address for the program to alter.

\param modelMatrix --- glm::mat4 that represents the model matrix.

*/

void ObjModel::setModelMatrix(GLuint program, glm::mat4 modelMatrix)
{
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "PVM"),
        1, GL_FALSE, glm::value_ptr(projection*view*model));

    glUniformMatrix4fv(glGetUniformLocation(program, "Model"),
        1, GL_FALSE, glm::value_ptr(model));
    glm::mat3 nM(model);
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(nM));
    glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"),
        1, GL_FALSE, glm::value_ptr(normalMatrix));
}

/**
\brief Sets the number of lights to use.

\param num --- the number of the lights.

*/

void ObjModel::setNumberOfLights(int num)
{
    if (num < 0)
        num = 0;
    if (num > MaxLights)
        num = MaxLights;

    numLights = num;

    for (unsigned int i = 0; i < programs.size(); i++)
    {
        glUseProgram(programs[i]);
        glUniform1i(glGetUniformLocation(programs[i], "numLights"), numLights);
    }
}


/**
\brief Sets a light value.

\param num --- the number of the light to set.

\param light --- the Light struct with the light information.

*/

void ObjModel::setLight(int num, Light light)
{
    if (num < 0 || num >= MaxLights)
        return;

    lt[num] = light;

    for (unsigned int i = 0; i < programs.size(); i++)
        LoadLight(programs[i], lt[num], "Lt", num);
}

/**
\brief Gets a light value.

\param num --- the number of the light to get.

\return Light object in position num.

*/

Light ObjModel::getLight(int num)
{
    Light retlight;
    if (num < 0 || num >= MaxLights)
        return retlight;

    return lt[num];
}

/**
\brief Sets the global ambient color.

\param ga --- glm::vec4 holding the (r, g, b, a) for the global ambient.

*/

void ObjModel::setGlobalAmbient(glm::vec4 ga)
{
    GlobalAmbient = ga;

    for (unsigned int i = 0; i < programs.size(); i++)
    {
        glUseProgram(programs[i]);
        glUniform4fv(glGetUniformLocation(programs[i], "GlobalAmbient"), 1, glm::value_ptr(GlobalAmbient));
    }
}

/**
\brief Sets the texture to material ratio, r to texture and (1-r) to material.

\param texR --- ratio value.

*/

void ObjModel::setTextureRatio(float texR)
{
    texRatio = texR;

    for (unsigned int i = 0; i < programs.size(); i++)
    {
        glUseProgram(programs[i]);
        glUniform1f(glGetUniformLocation(programs[i], "texRatio"), texRatio);
    }
}


/**
\brief Sets the position of the camera, used for lighting calculations.

\param Eye --- glm::vec3 holding the (x, y, z) position of the camera.

*/

void ObjModel::setEye(GLuint program, glm::vec3 Eye)
{
    glUseProgram(program);
    glUniform3fv(glGetUniformLocation(program, "eye"), 1, glm::value_ptr(eye));
}

/**
\brief Turns a light on.

\param num --- number of the light to turn on.

*/

void ObjModel::turnLightOn(int num)
{
    turnLightOnOff(num, true);
}

/**
\brief Turns a light off.

\param num --- number of the light to turn off.

*/

void ObjModel::turnLightOff(int num)
{
    turnLightOnOff(num, false);
}


/**
\brief Turns a light on or off.

\param num --- number of the light to turn off.

\param setting --- boolean for light value.

*/

void ObjModel::turnLightOnOff(int num, bool setting)
{
    if (num < 0 || num >= MaxLights)
        return;

    for (unsigned int i = 0; i < programs.size(); i++)
    {
        glUseProgram(programs[i]);
        char locID[100];
        sprintf(locID, "%s[%d].%s", "Lt", num, "on");
        glUniform1i(glGetUniformLocation(programs[i], locID), setting);
    }
}