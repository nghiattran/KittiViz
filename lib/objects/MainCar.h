#ifndef MAINCAR_H
#define MAINCAR_H

#include "Car.h"
#include "../utils/Material.h"
#include "../utils/MaterialPresets.h"
#include "../objects/Light.h"

class MainCar : public Car {
    public:
        MainCar();
        ~MainCar();
        void load();
    protected:
    private:
};

#endif // MAINCAR_H
