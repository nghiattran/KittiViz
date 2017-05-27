#ifndef GAUGE_H
#define GAUGE_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Circle.h"
#include "../objects/Ruler.h"
#include "../objects/TextRendererTTF.h"
#include "../utils/ProgramDefines.h"
#include "../utils/LoadShaders.h"
#include "../utils/Texture.h"
#include "../data/OXT.h"
#include "../patterns/Observer.h"
#include "../utils/TextureController.h"

class Gauge : public Observer<OXT>, public TextureController {
    public:
        Gauge(Color color);
        Gauge(Circle* c1, Circle* c2, Circle* c3);
        ~Gauge();

        void draw();
        virtual void update(OXT oxt) = 0;
    protected:
        glm::mat4 location;
        glm::mat4 levels[10];
    private:
        Circle* base;
        Circle* outerRing;
        Circle* innerRing;

        Screen* screen;

        void init();
};

#endif // GAUGE_H
