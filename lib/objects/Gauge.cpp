#include "Gauge.h"

Gauge::Gauge(Color mainColor)
{
    init();

    FadeStrategy* noFade = new FadeStrategy();
    InnerFade* innerFade = new InnerFade();

    Color black = Color(0, 0, 0);
    Color white = Color(1.0, 1.0, 1.0);

    base = new Circle(1.02, 1.02, 100, 2 * PI, black, noFade);
    outerRing = new Circle(1, 0.02, 100, 2 * PI, white, noFade);
    innerRing = new Circle(0.93, 0.3, 100, 2 * PI, mainColor, innerFade);
}

Gauge::Gauge(Circle* mbase, Circle* minnerRing, Circle* mouterRing)
{
    init();

    base = mbase;
    outerRing = mouterRing;
    innerRing = minnerRing;
}

Gauge::~Gauge() {
    delete base;
    delete outerRing;
    delete innerRing;
}

void Gauge::init()
{
    screen = Screen::getInstance();
    int height = screen->getHeight();
    int width = screen->getWidth();
    float screenRatio = (float) height / width;

    useProgram();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(-0.7, -0.5, 0));
    location = model;

    for (int i = 0; i < 10; ++i)
    {
        levels[i] = model;
        levels[i] = glm::translate(levels[i], glm::vec3(0, 0,  -0.001 * i));
        levels[i] = glm::scale(levels[i], glm::vec3(0.4 * screenRatio, 0.4, 0.2*screenRatio));
        levels[i] = glm::rotate(levels[i], 200*degf, glm::vec3(0, 0, 1));
    }

    useProgram();

    setModelMatrix(levels[0]);
}

void Gauge::draw()
{
    useProgram();
    init();
    turnOffTexture();

    glDisable(GL_BLEND);
    setModelMatrix(levels[0]);
    base->draw();

    glEnable(GL_BLEND);
    setModelMatrix(levels[1]);
    outerRing->draw();
    innerRing->draw();
}
