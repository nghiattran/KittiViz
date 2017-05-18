#ifndef OTHERCAR_H
#define OTHERCAR_H

#include "Car.h"

class OtherCar : public Car {
    public:
        OtherCar();
        virtual ~OtherCar();
        void load();
    protected:
    private:
};

#endif // OTHERCAR_H
