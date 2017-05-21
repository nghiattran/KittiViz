#include "SubWindow.h"

SubWindow::SubWindow()
{
    texCtrl = TextureController::getInstance();

    texCtrl->useProgram();

    model = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 0, 1));

    createTessellatedWallOBJ(1, 0.5, 1, 1);
    load(0, 1, 2, 3);
    setBorderColor(1, 1, 1);
    setDrawBorder(true);
}

SubWindow::~SubWindow()
{
    if (texture)
        delete texture;

    // TODO: fix double delete single
    // if (texCtrl)
    //     delete texCtrl;
}

/**
\brief Draw window
*/

void SubWindow::draw()
{
    texCtrl->turnTextureOn(texture);
    texCtrl->setModelMatrix(model);
    Models::draw();
    texCtrl->turnTextureOff(texture);
}

/**
\brief Set size.

\param h - Height.
\param w - Width.
*/

void SubWindow::setSize(float h, float w)
{
    createTessellatedWallOBJ(h, w, 1, 1);
    load(0, 1, 2, 3);
}


void SubWindow::loadTexture(sf::Image img)
{
    if (!texture)
    {
        texture = texCtrl->createNewTexture(img);
    } else {
        texture = texCtrl->updateTexture(texture, img);
    }
}

/**
\brief Set model matrix.

\param nmodel - target matrix.

*/

void SubWindow::setModelMatrix(glm::mat4 nmodel)
{
    model = nmodel;
    texCtrl->setModelMatrix(model);
}

/**
\brief Get model matrix.
*/

const glm::mat4 SubWindow::getModelMatrix()
{
    return model;
}
