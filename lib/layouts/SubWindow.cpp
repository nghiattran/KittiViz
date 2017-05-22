#include "SubWindow.h"

SubWindow::SubWindow()
{
    createTessellatedWallOBJ(1, 1, 1, 1);
    load(0, 1, 2, 3);
}

SubWindow::~SubWindow()
{
    //dtor
}


/**
\brief Draw road.
\param nprogram - program that is in used before drawing texture. This make sure everything is reset after draw texture.
*/

void SubWindow::draw()
{
    useProgram();
    Models::draw();
}

/**
\brief Set model matrix.
\param nmodel - target matrix.
*/

void SubWindow::setModelMatrix(glm::mat4 nmodel)
{
    // Flip image around x axis, to fix flipping error
    model = glm::rotate(nmodel, 180*degf, glm::vec3(1, 0, 0));
    TextureController::setModelMatrix(model);
}

/**
\brief Set size.
\parame int - number of lanes
*/

void SubWindow::setSize(float h, float w)
{
    createTessellatedWallOBJ(h, w, 1, 1);
    load(0, 1, 2, 3);
}
