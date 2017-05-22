#include "Gauge.h"

Gauge::Gauge()
{
    useProgram();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(-0.7, -0.5, 0));
    for (int i = 0; i < 10; ++i)
    {
        levels[i] = model;
        levels[i] = glm::scale(levels[i], glm::vec3(0.2, 0.4, 0.2));
        levels[i] = glm::translate(levels[i], glm::vec3(0, 0,  -0.001 * i));
        levels[i] = glm::rotate(levels[i], 200*degf, glm::vec3(0, 0, 1));
    }

    setModelMatrix(levels[0]);

    FadeStrategy* noFade = new FadeStrategy();
    InnerFade* innerFade = new InnerFade();
    // OneEndFade* oneEndFade = new OneEndFade(2, 0.1);

    Color black = Color(0, 0, 0);
    Color white = Color(1.0, 1.0, 1.0);
    Color fBlue = UintColor(44, 154,199);

    base = new Circle(1.02, 1.02, 100, 2 * PI, black, noFade);
    outerRing = new Circle(1, 0.02, 100, 2 * PI, white, noFade);
    innerRing = new Circle(0.93, 0.3, 100, 2 * PI, fBlue, innerFade);
    ruler = new Ruler(0.93, 0.10, (MAX_SPEED - 20) + 2, 10 * PI/9, white, noFade);

//    text.loadFont("fonts/segoesc.ttf");
//    text.setFontSize(26);
//    text.setColor(1, 1, 1, 1);
}

Gauge::~Gauge() {
    delete base;
    delete outerRing;
    delete innerRing;
    delete ruler;
//    delete text;
}

void Gauge::draw()
{
    useProgram();

    turnOffTexture();

    glDisable(GL_BLEND);
    setModelMatrix(levels[0]);
    base->draw();

    glEnable(GL_BLEND);
    setModelMatrix(levels[1]);
    outerRing->draw();
    innerRing->draw();

    setModelMatrix(levels[2]);
    ruler->draw();
}

void Gauge::update(OXT oxt)
{
    float speed = sqrt(pow(oxt.vf, 2) + pow(oxt.vl, 2) + pow(oxt.vu, 2));
    // Convert to MPH
    float speedMPH = speed * 2.23694;
    setSpeed(speedMPH);
    // printf("%f %f\n", oxt.vn, oxt.ve);
}

void Gauge::setSpeed(int s)
{
    speed = s;
    float lightingAngle = std::min((float) speed/MAX_SPEED, 1.0f) * ruler->getFanAngle();

    ruler->setLightingAngle(lightingAngle);
}
