#include "Models.h"

/**
\file Models.cpp
\brief Implementation file for the Models class.

\author    Don Spickler
\version   1.1
\date      Written: 2/28/2016  <BR> Revised: 2/28/2016

*/

/**
\brief Constructor, default.

*/

Models::Models()
{
    lon = 0;
    lat = 0;
    dat = NULL;
    normals = NULL;
    srand(static_cast<unsigned int>(clock()));

    vPosition = 0;
    vColor = 1;
    vNormal = 2;
    vTex = 3;

    reload = GL_FALSE;
    drawFaces = GL_TRUE;
    drawBorder = GL_TRUE;

    red = 1;
    green = 1;
    blue = 1;
    bred = 0;
    bgreen = 0;
    bblue = 0;
}

/**
\brief Destructor

Removes the data files on both the CPU and GPU.

*/

Models::~Models()
{
    if (dat != NULL) delete[] dat;
    if (normals != NULL) delete[] normals;

    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
    glBindVertexArray(vboptr2);
    glDeleteBuffers(1, &bufptr2);
    glDeleteBuffers(1, &eboptr2);
}

/**
\brief Resets the data array sizes.

\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::resetArraySize(GLint ilon, GLint ilat)
{
    if (dat != NULL) delete[] dat;
    if (normals != NULL) delete[] normals;

    lon = ilon;
    lat = ilat;
    dat = new GLfloat[3*(lon+1)*(lat+1)];
    normals = new GLfloat[3*(lon+1)*(lat+1)];
}

/**
\brief Dope vector for the conversion of a three dimensional
array into a one dimensional array.

\param i --- Dimension 1 index.
\param j --- Dimension 2 index.
\param k --- Dimension 3 index.

\return The position of the three dimensional (i, j, k) position in
the one dimensional array, i.e. 3*((lon+1)*j+i)+k.

*/

int Models::pos(int i, int j, int k)
{
    return 3*((lon+1)*j+i)+k;
}

/**
\brief Cross product function.

\param x1, y1, z1 --- First vector.
\param x2, y2, z2 --- Second vector.
\param cx, cy, cz --- Cross product vector (x1, y1, z1) X (x2, y2, z2).

*/

void Models::CrossProd(GLfloat x1, GLfloat y1, GLfloat z1,
                       GLfloat x2, GLfloat y2, GLfloat z2,
                       GLfloat *cx, GLfloat *cy, GLfloat *cz)
{
    *cx = y1*z2-y2*z1;
    *cy = -x1*z2+z1*x2;
    *cz = x1*y2-x2*y1;
}

/**
\brief Uses the data set to determine the normals at each vertex.  The algorithm
uses the 8 vertices around each vertex and the 8 triangles produced by each of
the consecutive vertices and averages the normals.

*/

void Models::createNormals()
{
    GLfloat v1x, v1y, v1z, v2x, v2y, v2z;
    GLfloat cx, cy, cz;
    GLfloat scx, scy, scz;
    int v1i, v1j, v2i, v2j;
    GLfloat mod;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            scx = 0;
            scy = 0;
            scz = 0;
            for (int k = 0; k < 8; k++)
            {
                switch (k)
                {
                case 0:
                    v1i = i+1;
                    v1j = j;
                    v2i = i+1;
                    v2j = j+1;
                    break;
                case 1:
                    v1i = i+1;
                    v1j = j+1;
                    v2i = i;
                    v2j = j+1;
                    break;
                case 2:
                    v1i = i;
                    v1j = j+1;
                    v2i = i-1;
                    v2j = j+1;
                    break;
                case 3:
                    v1i = i-1;
                    v1j = j+1;
                    v2i = i-1;
                    v2j = j;
                    break;
                case 4:
                    v1i = i-1;
                    v1j = j;
                    v2i = i-1;
                    v2j = j-1;
                    break;
                case 5:
                    v1i = i-1;
                    v1j = j-1;
                    v2i = i;
                    v2j = j-1;
                    break;
                case 6:
                    v1i = i;
                    v1j = j-1;
                    v2i = i+1;
                    v2j = j-1;
                    break;
                case 7:
                    v1i = i+1;
                    v1j = j-1;
                    v2i = i+1;
                    v2j = j;
                    break;
                }

                if ((v1i >= 0) && (v1i <= lon) && (v1j >= 0) && (v1j <= lat) && (v2i >= 0) && (v2i <= lon) && (v2j >= 0) && (v2j <= lat))
                {
                    v1x = dat[pos(v1i,v1j,0)] - dat[pos(i,j,0)];
                    v1y = dat[pos(v1i,v1j,1)] - dat[pos(i,j,1)];
                    v1z = dat[pos(v1i,v1j,2)] - dat[pos(i,j,2)];
                    v2x = dat[pos(v2i,v2j,0)] - dat[pos(i,j,0)];
                    v2y = dat[pos(v2i,v2j,1)] - dat[pos(i,j,1)];
                    v2z = dat[pos(v2i,v2j,2)] - dat[pos(i,j,2)];

                    CrossProd(v1x, v1y, v1z, v2x, v2y, v2z, &cx, &cy, &cz);
                    mod = sqrt(cx*cx + cy*cy + cz*cz);
                    if (mod > 0.000001)
                    {
                        scx += cx;
                        scy += cy;
                        scz += cz;
                    }
                }
            }
            mod = sqrt(scx*scx + scy*scy + scz*scz);
            if (mod > 0.000001)
            {
                normals[pos(i,j,0)] = scx/mod;
                normals[pos(i,j,1)] = scy/mod;
                normals[pos(i,j,2)] = scz/mod;
            }
            else
            {
                normals[pos(i,j,0)] = scx;
                normals[pos(i,j,1)] = scy;
                normals[pos(i,j,2)] = scz;
            }
        }
}

/**
\brief Reverses all of the normals on the normal array.

*/

void Models::reverseNormals()
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                normals[pos(i,j,k)] = -normals[pos(i,j,k)];
}

/**
\brief Sets the border drawing flag.

\param d --- New value of the border drawing flag.

*/

void Models::setDrawBorder(GLboolean d)
{
    drawBorder = d;
}

/**
\brief Sets the face drawing flag.

\param d --- New value of the face drawing flag.

*/

void Models::setDrawFaces(GLboolean d)
{
    drawFaces = d;
}

/**
\brief Sets the face color.

\param r --- Red component of the face.
\param g --- Green component of the face.
\param b --- Blue component of the face.

*/

void Models::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    red = r;
    green = g;
    blue = b;
    reloadData();
}

/**
\brief Sets the border color.

\param r --- Red component of the border.
\param g --- Green component of the border.
\param b --- Blue component of the border.

*/

void Models::setBorderColor(GLfloat r, GLfloat g, GLfloat b)
{
    bred = r;
    bgreen = g;
    bblue = b;
    reloadData();
}

/**
\brief Loads the object data to the graphics card.

*/

void Models::reloadData()
{
    load(vPosition, vColor, vNormal, vTex);
    reload = GL_TRUE;
}

/**
\brief Loads the object data to the graphics card.

\param v --- Shader position of the vertex data.
\param c --- Shader position of the vertex color.
\param n --- Shader position of the vertex normal.
\param t --- Shader position of the texture coordinates.

*/

void Models::load(GLint v, GLint c, GLint n, GLint t)
{
    vPosition = v;
    vColor = c;
    vNormal = n;
    vTex = t;

    GLuint *indices;
    GLuint *border_indices;
    GLfloat *points;
    GLfloat *colors;
    GLfloat *normalvectors;
    GLfloat *border_colors;
    GLfloat *tex;

    indices = new GLuint[6*(lon)*(lat)];
    border_indices = new GLuint[4*(lon)*(lat)+2*(lon+lat)];
    points = new GLfloat[4*(lon+1)*(lat+1)];
    normalvectors = new GLfloat[3*(lon+1)*(lat+1)];
    colors = new GLfloat[4*(lon+1)*(lat+1)];
    border_colors = new GLfloat[4*(lon+1)*(lat+1)];
    tex = new GLfloat[2*(lon+1)*(lat+1)];

    for (int i = 0; i < (lon+1)*(lat+1); i++)
    {
        colors[4*i] = red;
        colors[4*i+1] = green;
        colors[4*i+2] = blue;
        colors[4*i+3] = 1;
        border_colors[4*i] = bred;
        border_colors[4*i+1] = bgreen;
        border_colors[4*i+2] = bblue;
        border_colors[4*i+3] = 1;
    }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            tex[2*i*(lat+1)+2*j] = 1.0*i/lon;
            tex[2*i*(lat+1)+2*j+1] = 1.0*j/lat;
        }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            points[4*i*(lat+1)+4*j] = dat[pos(i,j,0)];
            points[4*i*(lat+1)+4*j+1] = dat[pos(i,j,1)];
            points[4*i*(lat+1)+4*j+2] = dat[pos(i,j,2)];
            points[4*i*(lat+1)+4*j+3] = 1;
        }

    for (int i = 0; i < lon+1; i++)
        for (int j = 0; j < lat+1; j++)
        {
            normalvectors[3*i*(lat+1)+3*j] = normals[pos(i,j,0)];
            normalvectors[3*i*(lat+1)+3*j+1] = normals[pos(i,j,1)];
            normalvectors[3*i*(lat+1)+3*j+2] = normals[pos(i,j,2)];
        }

    int indexarraypos = 0;
    for (int i = 0; i < lon; i++)
        for (int j = 0; j < lat; j++)
        {
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j;
            indices[indexarraypos++] = (i+1)*(lat+1)+j+1;
            indices[indexarraypos++] = i*(lat+1)+j+1;;
        }

    indexarraypos = 0;
    for (int i = 0; i < lon; i++)
        for (int j = 0; j < lat; j++)
        {
            border_indices[indexarraypos++] = i*(lat+1)+j;
            border_indices[indexarraypos++] = (i+1)*(lat+1)+j;
            border_indices[indexarraypos++] = i*(lat+1)+j;
            border_indices[indexarraypos++] = i*(lat+1)+j+1;
        }

    for (int i = 0; i < lon; i++)
    {
        border_indices[indexarraypos++] = i*(lat+1)+lat;
        border_indices[indexarraypos++] = (i+1)*(lat+1)+lat;
    }

    for (int j = 0; j < lat; j++)
    {
        border_indices[indexarraypos++] = lon*(lat+1)+j;
        border_indices[indexarraypos++] = lon*(lat+1)+j+1;
    }

    if (!reload)
        glGenVertexArrays(1, &vboptr);

    glBindVertexArray(vboptr);

    if (!reload)
        glGenBuffers(1, &eboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*lon*lat*sizeof(GLuint), indices, GL_DYNAMIC_DRAW);

    if (!reload)
        glGenBuffers(1, &bufptr);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 3*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 2*(lon+1)*(lat+1)*sizeof(GLfloat),
                 NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*(lon+1)*(lat+1)*sizeof(GLfloat), points);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat),
                    4*(lon+1)*(lat+1)*sizeof(GLfloat), colors);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 4*(lon+1)*(lat+1)*sizeof(GLfloat),
                    3*(lon+1)*(lat+1)*sizeof(GLfloat), normalvectors);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 3*(lon+1)*(lat+1)*sizeof(GLfloat),
                    2*(lon+1)*(lat+1)*sizeof(GLfloat),
                    tex);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 3*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);

    if (!reload)
        glGenVertexArrays(1, &vboptr2);

    glBindVertexArray(vboptr2);

    if (!reload)
        glGenBuffers(1, &eboptr2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (4*(lon)*(lat)+2*(lon+lat))*sizeof(GLuint),
                 border_indices, GL_DYNAMIC_DRAW);

	if (!reload)
        glGenBuffers(1, &bufptr2);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr2);
    glBufferData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 3*(lon+1)*(lat+1)*sizeof(GLfloat)
                 + 2*(lon+1)*(lat+1)*sizeof(GLfloat),
                 NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 4*(lon+1)*(lat+1)*sizeof(GLfloat), points);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat),
                    4*(lon+1)*(lat+1)*sizeof(GLfloat), border_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 4*(lon+1)*(lat+1)*sizeof(GLfloat),
                    3*(lon+1)*(lat+1)*sizeof(GLfloat), normalvectors);
    glBufferSubData(GL_ARRAY_BUFFER, 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                    + 3*(lon+1)*(lat+1)*sizeof(GLfloat),
                    2*(lon+1)*(lat+1)*sizeof(GLfloat),
                    tex);

    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 3*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_TRUE, 0,
                          BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)
                                        + 4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0,
                          BUFFER_OFFSET(4*(lon+1)*(lat+1)*sizeof(GLfloat)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTex);

    if (indices) delete indices;
    if (border_indices) delete border_indices;
    if (points) delete points;
    if (normalvectors) delete normalvectors;
    if (colors) delete colors;
    if (border_colors) delete border_colors;
    if (tex) delete tex;

    reload = GL_FALSE;
}

/**
\brief Draws the object.

*/

void Models::draw()
{
    if (drawFaces)
    {
        glBindVertexArray(vboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glDrawElements(GL_TRIANGLES, 6*lon*lat, GL_UNSIGNED_INT, NULL);
    }

    if (drawBorder)
    {
        glBindVertexArray(vboptr2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr2);
        glLineWidth(2);
        glDrawElements(GL_LINES, 4*(lon)*(lat)+2*(lon+lat), GL_UNSIGNED_INT, NULL);
        glLineWidth(1);
    }
}

/**
\brief Creates a sphere.

\param r --- Radius of the sphere.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createSphereOBJ(GLfloat r, GLint ilon, GLint ilat)
{
    createPartialSphereOBJ(r, ilon, ilat, 0, 2*PI, -PI/2, PI/2);
}


/**
\brief Creates a cloud points.

\param points --- All points.

*/

// void Models::createCloudPoints(std::vector<float> points) {
    
// }

/**
\brief Creates a partial sphere.

\param r --- Radius of the sphere.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.
\param begintheta --- Beginning theta angle of the sphere object.
\param endtheta --- Ending theta angle of the sphere object.
\param beginpsi --- Beginning psi angle of the sphere object.
\param endpsi --- Ending psi angle of the sphere object.

*/

void Models::createPartialSphereOBJ(GLfloat r, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsi, GLfloat endpsi)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = begintheta + (endtheta-begintheta)*((float)i/lon);
            GLfloat psi = beginpsi + (endpsi-beginpsi)*((float)j/lat);

            dat[pos(i,j,0)] = r*cos(psi)*cos(theta);
            dat[pos(i,j,1)] = r*cos(psi)*sin(theta);
            dat[pos(i,j,2)] = r*sin(psi);
            normals[pos(i,j,0)] = dat[pos(i,j,0)]/r;
            normals[pos(i,j,1)] = dat[pos(i,j,1)]/r;
            normals[pos(i,j,2)] = dat[pos(i,j,2)]/r;
        }
}

/**
\brief Creates a torus.

\param irad --- Inner radius of the torus.
\param irad --- Outer radius of the torus.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat)
{
    createPartialTorusOBJ(irad, orad, ilon, ilat, -PI, PI, -PI, PI);
}

/**
\brief Creates a partial torus.

\param irad --- Inner radius of the torus.
\param irad --- Outer radius of the torus.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.
\param begintheta --- Beginning theta angle of the torus object.
\param endtheta --- Ending theta angle of the torus object.
\param beginpsi --- Beginning psi angle of the torus object.
\param endpsi --- Ending psi angle of the torus object.

*/

void Models::createPartialTorusOBJ(GLfloat irad, GLfloat orad, GLint ilon, GLint ilat, GLfloat begintheta, GLfloat endtheta, GLfloat beginpsi, GLfloat endpsi)
{
    GLfloat r = (orad - irad)/2;
    GLfloat rax = r+irad;
    resetArraySize(ilon, ilat);
    GLdouble cx, cy, cz;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = begintheta + (endtheta-begintheta)*((float)i/lon);
            GLfloat psi = endpsi - (beginpsi + (endpsi-beginpsi)*((float)j/lat));

            dat[pos(i,j,0)] = (rax-r*cos(psi))*cos(theta);
            dat[pos(i,j,1)] = (rax-r*cos(psi))*sin(theta);
            dat[pos(i,j,2)] = r*sin(psi);

            cx = -r*cos(psi)*cos(theta);
            cy = -r*cos(psi)*sin(theta);
            cz = r*sin(psi);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates a trefoil.

\param amp --- Amplitude of the trefoil.
\param numtwists --- Number of twists.
\param rmaj --- Major radius.
\param rmin --- Minor radius.
\param elongfact --- Elongation factor.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createTrefoilOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 4*PI-4*PI*((float)i/lon);
            GLfloat psi = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = rmin*cos(psi)*cos(theta)+rmaj*cos(theta)*(1+amp*cos(numtwists*theta));
            dat[pos(i,j,1)] = elongfact*rmin*sin(psi)+amp*sin(numtwists*theta);
            dat[pos(i,j,2)] = rmin*cos(psi)*sin(theta)+rmaj*sin(theta)*(1+amp*cos(numtwists*theta));

            x2 = -rmin*cos(psi)*sin(theta)-rmaj*sin(theta)*(1+amp*cos(numtwists*theta))-rmaj*cos(theta)*(numtwists*amp*sin(numtwists*theta));
            y2 = numtwists*amp*cos(numtwists*theta);
            z2 = rmin*cos(psi)*cos(theta)+rmaj*cos(theta)*(1+amp*cos(numtwists*theta))-rmaj*sin(theta)*(numtwists*amp*sin(numtwists*theta));
            x1 = -rmin*sin(psi)*cos(theta);
            y1 = elongfact*rmin*cos(psi);
            z1 = -rmin*sin(psi)*sin(theta);

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates an umbilic torus.

\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createUmbilicTorusOBJ(GLint ilon, GLint ilat)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat psi = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = sin(theta)*(7+cos(theta/3-2*psi)+2*cos(theta/3+psi));
            dat[pos(i,j,1)] = cos(theta)*(7+cos(theta/3-2*psi)+2*cos(theta/3+psi));
            dat[pos(i,j,2)] = sin(theta/3-2*psi)+2*sin(theta/3+psi);
        }
    createNormals();
}

/**
\brief Creates a braided torus.

\param amp --- Amplitude of the trefoil.
\param numtwists --- Number of twists.
\param rmaj --- Major radius.
\param rmin --- Minor radius.
\param elongfact --- Elongation factor.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createBraidedTorusOBJ(GLfloat amp, GLfloat numtwists, GLfloat rmaj, GLfloat rmin, GLfloat elongfact, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 8*PI - 8*PI*((float)i/lon);
            GLfloat psi = 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = rmin*cos(psi)*cos(theta)+rmaj*cos(theta)*(1+amp*cos(numtwists*theta));
            dat[pos(i,j,1)] = elongfact*(rmin*sin(psi)+amp*sin(numtwists*theta));
            dat[pos(i,j,2)] = rmin*cos(psi)*sin(theta)+rmaj*sin(theta)*(1+amp*cos(numtwists*theta));

            x1 = -rmin*sin(psi)*cos(theta);
            y1 = elongfact*rmin*cos(psi);
            z1 = -rmin*sin(psi)*sin(theta);

            x2 = -rmin*cos(psi)*sin(theta)-rmaj*sin(theta)*(1+amp*cos(numtwists*theta))-rmaj*cos(theta)*(numtwists*amp*sin(numtwists*theta));
            y2 = elongfact*(numtwists*amp*cos(numtwists*theta));
            z2 = rmin*cos(psi)*cos(theta)+rmaj*cos(theta)*(1+amp*cos(numtwists*theta))-rmaj*sin(theta)*(numtwists*amp*sin(numtwists*theta));

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates an exponential horn.

\param r1 --- Radius 1.
\param r2 --- Radius 2.
\param len --- Length
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createExpHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat x = len*((float)j/lat);
            GLfloat Rad = r1*exp(log(r2/r1)/len*x);

            dat[pos(i,j,0)] = Rad*sin(theta);
            dat[pos(i,j,1)] = Rad*cos(theta);
            dat[pos(i,j,2)] = x;

            x1 = log(r2/r1)/len*r1*exp(log(r2/r1)/len*x)*sin(theta);
            y1 = log(r2/r1)/len*r1*exp(log(r2/r1)/len*x)*cos(theta);
            z1 = 1;

            x2 = Rad*cos(theta);
            y2 = -Rad*sin(theta);
            z2 = 0;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates a quadratic horn.

\param r1 --- Radius 1.
\param r2 --- Radius 2.
\param len --- Length
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createQuadHornOBJ(GLfloat r1, GLfloat r2, GLfloat len, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 2*PI - 2*PI*((float)i/lon);
            GLfloat x = len*((float)j/lat);
            GLfloat Rad = (r2-r1)/(len*len)*(x*x) + r1;

            dat[pos(i,j,0)] = Rad*sin(theta);
            dat[pos(i,j,1)] = Rad*cos(theta);
            dat[pos(i,j,2)] = x;

            x1 = 2.0*(r2-r1)/(len*len)*x*sin(theta);
            y1 = 2.0*(r2-r1)/(len*len)*x*cos(theta);
            z1 = 1;

            x2 = Rad*cos(theta);
            y2 = -Rad*sin(theta);
            z2 = 0;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates a helical torus.

\param irad --- Inner radius of the torus.
\param irad --- Outer radius of the torus.
\param strFact --- Stretch factor.
\param begang --- Beginning angle.
\param endang --- Ending angle.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createHelicalTorusOBJ(GLfloat irad, GLfloat orad, GLfloat strFact, GLfloat begang, GLfloat endang, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);
    GLfloat r = (orad - irad)/2;
    GLfloat rax = r+irad;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = endang - (begang + (endang - begang)*((float)i/lon));
            GLfloat psi = -PI + 2*PI*((float)j/lat);

            dat[pos(i,j,0)] = (rax-r*cos(psi))*cos(theta);
            dat[pos(i,j,1)] = (rax-r*cos(psi))*sin(theta);
            dat[pos(i,j,2)] = strFact*theta + r*sin(psi);

            x1 = r*sin(psi)*cos(theta);
            y1 = r*sin(psi)*sin(theta);
            z1 = r*cos(psi);

            x2 = -sin(theta);
            y2 = cos(theta);
            z2 = strFact;

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates a Mobius strip.

\param begv --- Start to the v parameter range.
\param endv --- End to the v parameter range.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createMobiusOBJ(GLfloat begv, GLfloat endv, GLint ilon, GLint ilat)
{
    GLfloat cx, cy, cz;
    GLfloat x1, y1, z1;
    GLfloat x2, y2, z2;
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat theta = 2*PI*((float)i/lon);
            GLfloat v = begv + (endv-begv)*((float)j/lat);

            dat[pos(i,j,0)] = cos(theta)+v*cos(theta/2)*cos(theta);
            dat[pos(i,j,1)] = sin(theta)+v*cos(theta/2)*sin(theta);
            dat[pos(i,j,2)] = v*sin(theta/2);

            x2 = cos(theta/2)*cos(theta);
            y2 = cos(theta/2)*sin(theta);
            z2 = sin(theta/2);

            x1 = -sin(theta)+v*(-cos(theta/2)*sin(theta)-0.5*sin(theta/2)*cos(theta));
            y1 = cos(theta)+v*(cos(theta/2)*cos(theta)-0.5*sin(theta/2)*sin(theta));
            z1 = 0.5*v*cos(theta/2);

            CrossProd(x1, y1, z1, x2, y2, z2, &cx, &cy, &cz);

            GLdouble mod = sqrt(cx*cx + cy*cy + cz*cz);
            normals[pos(i,j,0)] = cx/mod;
            normals[pos(i,j,1)] = cy/mod;
            normals[pos(i,j,2)] = cz/mod;
        }
}

/**
\brief Creates a tessellated wall.

\param width --- Width of the plane.
\param height --- Height of the plane.
\param ilon --- Number of longitudinal divisions.
\param ilat --- Number of latitude divisions.

*/

void Models::createTessellatedWallOBJ(GLfloat width, GLfloat height, GLint ilon, GLint ilat)
{
    resetArraySize(ilon, ilat);

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            dat[pos(i,j,0)] = -width/2.0+(float)i/(float)lon*width;
            dat[pos(i,j,1)] = -height/2.0+(float)j/(float)lat*height;
            dat[pos(i,j,2)] = 0;

            normals[pos(i,j,0)] = 0;
            normals[pos(i,j,1)] = 0;
            normals[pos(i,j,2)] = 1;
        }
}

/**
\brief Adds a random perturbation to the vertices in the model.

\param a --- Maximum perturbation.

*/

void Models::perturbVeritces(float a)
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                dat[pos(i,j,k)] += (double(rand())/double(RAND_MAX)-0.5)*a;

    createNormals();
}

/**
\brief Adds a random perturbation to the normals in the model.

\param a --- Maximum perturbation.

*/

void Models::perturbNormalsRandom(float a)
{
    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
            for (int k = 0; k < 3; k++)
                normals[pos(i,j,k)] += (double(rand())/double(RAND_MAX)-0.5)*a;

    for (int i = 0; i <= lon; i++)
        for (int j = 0; j <= lat; j++)
        {
            GLfloat mod = sqrt(normals[pos(i,j,0)]*normals[pos(i,j,0)] +
                               normals[pos(i,j,1)]*normals[pos(i,j,1)] +
                               normals[pos(i,j,2)]*normals[pos(i,j,2)]);

            if (mod < 0.00001) mod = 0.00001;
            normals[pos(i,j,0)] /= mod;
            normals[pos(i,j,1)] /= mod;
            normals[pos(i,j,2)] /= mod;
        }
}
