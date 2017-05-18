#ifndef __Models_h_
#define __Models_h_

#include <GL/glew.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../utils/ProgramDefines.h"

/**
\file Models.h
\brief Header file for Models.cpp

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\class Models

\brief The Models class is simply a class for easy construction of a few 3-D
mathematical objects.

Use of this class is straight forward.  Declare a Models object, call one of the create
methods to generate the data sets, call the load method to load the data to the graphics
card (the three parameters are the data positions of the vertices, colors, and normals in
that order, which are expected by the shader), and then in your display function call the
draw method.

~~~~~~~~~~~~~~~{.c}
Models obj;

obj.createSphereOBJ(0.25, 7, 7);
obj.load(0, 1, 2);

obj.draw();
~~~~~~~~~~~~~~~


*/

class Models
{
private:
    GLint lon;         ///< Number of longitudinal divisions.
    GLint lat;         ///< Number of latitude divisions.
    GLfloat *dat;      ///< Pointer to the vertex data.
    GLfloat *normals;  ///< Pointer to the normal data.

    GLint vPosition;   ///< Shader position of the vertex data.
    GLint vColor;      ///< Shader position of the vertex color.
    GLint vNormal;     ///< Shader position of the vertex normal.
    GLint vTex;        ///< Shader position of the texture coordinate.

    GLboolean reload;      ///< Flag to reload the data to the graphics card.
    GLboolean drawFaces;   ///< Flag to draw the faces of the object.
    GLboolean drawBorder;  ///< Flag to draw the border of the object.

    GLfloat red;    ///< Red component of the object color.
    GLfloat green;  ///< Green component of the object color.
    GLfloat blue;   ///< Blue component of the object color.
    GLfloat bred;   ///< Red component of the object border color.
    GLfloat bgreen; ///< Green component of the object border color.
    GLfloat bblue;  ///< Blue component of the object border color.

    GLuint vboptr;  ///< ID for the VBO.
    GLuint bufptr;  ///< ID for the data buffer.
    GLuint eboptr;  ///< ID for the index buffer.
    GLuint vboptr2; ///< ID for the VBO for the border.
    GLuint bufptr2; ///< ID for the data buffer for the border.
    GLuint eboptr2; ///< ID for the index buffer for the border.

    void CrossProd(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat *cx, GLfloat *cy, GLfloat *cz);
    void resetArraySize(GLint ilon, GLint ilat);
    int pos(int i, int j, int k);

public:
    Models();
    ~Models();

    void load(GLint v, GLint c, GLint n, GLint t);
    void reloadData();
    void draw();

    void setDrawBorder(GLboolean d);
    void setDrawFaces(GLboolean d);
    void setColorCube(GLboolean d);

    void setColor(GLfloat r, GLfloat g, GLfloat b);
    void setBorderColor(GLfloat r, GLfloat g, GLfloat b);

    void createNormals();
    void reverseNormals();
    void createSphereOBJ(GLfloat r, GLint ilon, GLint ilat);
    void createPartialSphereOBJ(GLfloat r, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy);
    void createTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat);
    void createPartialTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsy, GLfloat endpsy);
    void createTrefoilOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint lon, GLint lat);
    void createUmbilicTorusOBJ(GLint ilon, GLint ilat);
    void createBraidedTorusOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat);
    void createExpHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat);
    void createQuadHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat);
    void createHelicalTorusOBJ(GLfloat irad, GLfloat orad, GLfloat strFact, GLfloat begang, GLfloat endang, GLint ilon, GLint ilat);
    void createMobiusOBJ(GLfloat begv, GLfloat endv, GLint ilon, GLint ilat);
    void createTessellatedWallOBJ(GLfloat width, GLfloat height, GLint ilon, GLint ilat);

    void createCloudPoints();

    void perturbVeritces(float a);
    void perturbNormalsRandom(float a);
};

#endif
