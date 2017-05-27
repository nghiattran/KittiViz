#include "SubWindow.h"

SubWindow* SubWindow::mInstance = NULL;

SubWindow::SubWindow()
{
    for (int i = 0; i < NUM_CAMERA; i++)
    {
        float h = 1;
        float w = 2;
        float cx = -0.5 + 1 * i;
        float cy = 0;

        glm::mat4 subwindowModel = glm::translate(glm::mat4(1.0), glm::vec3(cx, cy, 0));

        cameraImages[i].setSize(h, w);
        cameraImages[i].setModelMatrix(subwindowModel);
    }
}

SubWindow::~SubWindow()
{
    //dtor
}

SubWindow* SubWindow::getInstance()
{
    if (!mInstance)
    {
        mInstance = new SubWindow();
    }
    return mInstance;
}

void SubWindow::draw()
{
    for (int i = 0; i < NUM_CAMERA; i++)
    {
        cameraImages[i].draw();
    }
}

void SubWindow::update(ImageData data)
{
    std::vector<sf::Image> images = data.getData();
    for (int i = 0; i < std::min(4, (int)images.size()); i++)
    {
        cameraImages[i].loadTexture(images[i]);
    }
}
