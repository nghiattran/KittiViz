#ifndef CAR_H
#define CAR_H

#include <GL/glew.h>

#include "../objects/ObjModel.h"

class Car : public ObjModel {
    public:
        Car();
        ~Car();
        virtual void load()  = 0;
    protected:
    private:
};

#endif // CAR_H
