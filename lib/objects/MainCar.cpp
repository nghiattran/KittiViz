#include "MainCar.h"

MainCar::MainCar() : Car()
{
}

void MainCar::load() {
    Load("Models/Camaro 2009/", "Chevrolet Camaro 2009.obj");
    setTextureRatio(0.1);

    glm::mat4 carModel = glm::rotate(glm::mat4(1.0), 90*degf, glm::vec3(0, 1, 0));
    carModel = glm::translate(carModel, glm::vec3(0, -1, 0));
    float scale = 0.3;
    carModel = glm::scale(carModel, glm::vec3(scale, scale, scale));
    setModelMatrix(carModel);
}

MainCar::~MainCar()
{
    //dtor
}
