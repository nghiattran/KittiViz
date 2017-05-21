#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "teapot.h"
#include "lib/objects/Axes.h"
#include "lib/loaders/PointsLoader.h"
#include "lib/loaders/DataLoader.h"
#include "lib/loaders/BoxLoader.h"
#include "lib/cameras/SphericalCamera.h"
#include "lib/cameras/YPRCamera.h"
#include "lib/objects/Models.h"
#include "lib/objects/MainCar.h"
#include "lib/objects/BoundingBox.h"
#include "lib/objects/Gauge.h"
#include "lib/layouts/SubWindow.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Don Spickler, Nghia Tran
\version   1.2
\date      Written: 4/5/2016  <BR> Revised: 4/6/2016

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow {
    private:
        GLenum mode;    ///< Mode, either point, line or fill.
        int sscount;    ///< Screenshot count to be appended to the screenshot filename.
        bool isDrawCloudpoints = true;
        const int NUM_LIGHT = 3;
        static const int NUM_SUBWINDOW = 2;

        Axes coords;    ///< Axes Object
        PointsLoader* pointsLoader;      ///< Object to load velodyne cloud points.
        DataLoader* dataLoader;          ///< Object to control all data processing.
        BoxLoader* boxLoader;            ///< Object to control all bouding boxes.

        GLuint ModelLoc;       ///< Location ID of the Model matrix in the shader.
        GLuint NormalLoc;      ///< Location ID of the Normal matrix in the shader.
        GLuint PVMLoc;         ///< Location ID of the PVM matrix in the shader.
        GLuint program;        ///< ID of the shader program.
        GLuint CMprogram;      ///< ID of the cube map shader program.
        GLuint Rprogram;       ///< ID of the road shader program.
        GLuint texTransLoc;    ///< Location ID of the texture transformation

        SphericalCamera sphcamera;   ///< Spherical Camera
        YPRCamera yprcamera;         ///< Yaw-Pitch-Roll Camera
        int CameraNumber;            ///< Camera number 1 = spherical, 2 = yaw-pitch-roll.
        SphericalCamera LtPos[10];   ///< Spherical "Camera" to control position of the light.
        Gauge gauge;

        Material mat;         ///< Material of the current object.
        Light lt[10];         ///< Light object.

        Models lightobj;      ///< Used for sphere at light source.
        MainCar mainCar;    ///< Object wrapper for Wavefront obj file data.

        GLuint CubeMapTexId;  ///< Cube Map Texture ID.
        Models CMSphere;      ///< Sphere Object for Cube Map
        SubWindow subwindows[NUM_SUBWINDOW]; ///< SubWindow Objects

        glm::mat4 projection;  ///< Projection Matrix
        glm::mat4 model;       ///< Model Matrix

        GLboolean isDrawAxes;    ///< Boolean for axes being drawn.
        GLboolean isPlaying;   ///< Boolean for playing vidoes.

        sf::Image texture;     ///< Cube map texture.

        void screenshot(std::string ext);
        void printOpenGLErrors();
        void print_GLM_Matrix(glm::mat4 m);
        void print_GLM_Matrix(glm::mat3 m);
    public:
        GraphicsEngine(std::string, GLint, GLint);
        ~GraphicsEngine();

        int fps;

        void display();
        void changeMode();
        void screenshotPNG();
        void screenshotJPG();
        void resize();
        void setSize(unsigned int, unsigned int);
        GLfloat* getScreenBounds();

        GLboolean isSphericalCameraOn();
        void setSphericalCameraOn();
        GLboolean isYPRCameraOn();
        void setYPRCameraOn();

        // void loadLight(Light Lt);
        // void loadMaterial(Material Mat);

        // void turnLightOn();
        // void turnLightOff();
        // void turnLightOn(std::string name, int i);
        // void turnLightOff(std::string name, int i);
        // void turnLightsOn(std::string name, int num);
        // void turnLightsOff(std::string name, int num);

        // void LoadLights(Light Lt[], std::string name, int num);
        // void LoadLight(Light Lt, std::string name, int i);

        void setDrawAxes(GLboolean b);
        void toggleDrawAxes();

        void setPlayingVideo(GLboolean b);
        void togglePlayingVideo();

        void toggleDrawCloudpoints();

        void setFrameRate(int frameRate);
        int getFrameRate() const;
        void increaseFrameRate(int diff);

        void setupSideWindows();
        void drawSideWindows();

        SphericalCamera* getLtPos();
        SphericalCamera* getSphericalCamera();
        YPRCamera* getYPRCamera();
};

#endif // GRAPHICSENGINE_H_INCLUDED
