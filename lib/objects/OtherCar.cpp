#include "OtherCar.h"

OtherCar::OtherCar() : Car()
{
    //ctor
}

void OtherCar::load() {
  Load("Models/Camaro 2009/", "Chevrolet Camaro 2009.obj");
  setTextureRatio(0.3);
}

OtherCar::~OtherCar()
{
    //dtor
}
