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
#include "../utils/OXT.h"
#include "../utils/TextureController.h"

class Gauge : public TextureController {
    public:
        Gauge();
        ~Gauge();

        void draw();
        Circle* base;
        Circle* outerRing;
        Circle* innerRing;
        Ruler* ruler;

        void setSpeed(int s);
        int getSpeed();

        void update(OXT oxt);
    protected:
    private:
        glm::mat4 levels[10];
        int speed = 0;
        const int MAX_SPEED = 160;
        TextRendererTTF text;

        // void PlotText(TextRenderer tr, std::string s);
};

#endif // GAUGE_H
