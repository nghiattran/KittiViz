#include "MainCar.h"

MainCar::MainCar() : Car()
{
}

void MainCar::load() {
  Load("Models/Camaro 2009/", "Chevrolet Camaro 2009.obj");
  setTextureRatio(0.3);
  glm::mat4 carModel = glm::translate(glm::mat4(1.0), glm::vec3(0, 3, 0));
  setModelMatrix(carModel);
}

MainCar::~MainCar()
{
    //dtor
}
