#ifndef SubWindow_H
#define SubWindow_H


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

#include "../patterns/Observer.h"
#include "../data/ImageData.h"
#include "../layouts/CameraImage.h"

class SubWindow : public Observer<ImageData>
{
    public:
        static SubWindow* getInstance();
        ~SubWindow();

        void draw();
    protected:
    private:
        SubWindow();

        static const int NUM_CAMERA = 2;
        CameraImage cameraImages[NUM_CAMERA];

        static SubWindow* mInstance;

        void update(ImageData data);       ///< Implementation of observer pattern.
};
#endif // SubWindow_H
