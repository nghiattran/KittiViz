#ifndef SPEEDOMETER_H
#define SPEEDOMETER_H

#include <typeinfo>

#include "../objects/Gauge.h"

struct SpeedUnit
{
    const std::string UNIT_NAME;
    const float MAX_SPEED;
    const float FACTOR;

    glm::mat4 location;

    SpeedUnit(std::string n, float maxSpeed, float factor, Ruler* ruler = nullptr) : UNIT_NAME(n), MAX_SPEED(maxSpeed), ruler(ruler), FACTOR(factor)
    {
        SPEED_TEXT.setFontSize(100);
        SPEED_TEXT.setColor(1, 1, 1, 1);

        UNIT_TEXT.setFontSize(100);
        UNIT_TEXT.setColor(1, 1, 1, 1);

        setLocationMatrix(glm::mat4(1.0));
    };

    ~SpeedUnit()
    {
        delete ruler;
    }

    float convert(float speed)
    {
        return speed * FACTOR;
    }

    float reverse(float speed)
    {
        return speed / FACTOR;
    }

    virtual float updateSpeed(float mspeed)
    {
        speed = convert(mspeed);

        // Update ruler's lingting
        float lightingAngle = std::min((float) speed/MAX_SPEED, 1.0f) * ruler->getFanAngle();
        ruler->setLightingAngle(lightingAngle);

        // Set up unit texts location
        int width = SPEED_TEXT.textWidth(std::to_string(speed));
        glm::mat4 speedTextModel = glm::translate(location, glm::vec3(-width/1000.0, -0.05, -0.1));
        speedTextModel = glm::scale(speedTextModel, glm::vec3(.002, .002, 0));
        SPEED_TEXT.setModelMatrix(speedTextModel);
    };

    virtual void draw()
    {
        ruler->draw();
        UNIT_TEXT.draw(UNIT_NAME);
        SPEED_TEXT.draw(std::to_string(speed));
    }

    void setRuler(Ruler* mruler)
    {
        if (ruler) delete ruler;

        ruler = mruler;
    }

    float getSpeed()
    {
        return speed;
    }

    void setLocationMatrix(glm::mat4 loc)
    {
        location = loc;

        if (ruler)
            ruler->loadTexts(location);

        // Set up unit texts location
        int width = UNIT_TEXT.textWidth(UNIT_NAME);
        glm::mat4 unitTextModel = glm::translate(location, glm::vec3(-width/3000.0, -0.13, -0.1));
        unitTextModel = glm::scale(unitTextModel, glm::vec3(.0006, .0006, 0));
        UNIT_TEXT.setModelMatrix(unitTextModel);
    }

    private:
        int speed;
        Ruler* ruler;
        TextRendererTTF SPEED_TEXT = TextRendererTTF("fonts/Tunga Bold.ttf");
        TextRendererTTF UNIT_TEXT = TextRendererTTF("fonts/Tunga Bold.ttf");
};

struct MPHUnit : SpeedUnit
{
    MPHUnit() : SpeedUnit("MPH", MAX_SPEED, 2.23694)
    {
        Color white = Color(1, 1, 1);
        FadeStrategy noFade = FadeStrategy();
        Ruler* ruler = new Ruler(0.93, 0.10, MAX_SPEED + 2, 10 * PI/9, white, noFade);
        ruler->loadTexts(location);
        setRuler(ruler);
    };

    private:
        static const int MAX_SPEED = 160;
};

struct KPHUnit : SpeedUnit
{
    KPHUnit() : SpeedUnit("KPH", MAX_SPEED, 3.6)
    {
        Color white = Color(1, 1, 1);
        FadeStrategy noFade = FadeStrategy();
        Ruler* ruler = new Ruler(0.93, 0.10, MAX_SPEED + 2, 10 * PI / 9, white, noFade);
        ruler->loadTexts(location);
        setRuler(ruler);
    };

    private:
        static const int MAX_SPEED = 240;
};

class Speedometer : public Gauge
{
    public:
        Speedometer();
        ~Speedometer();

        float getSpeed();

        void draw();

        void toggleUnit();

        virtual void update(OXT oxt) override;
    protected:

    private:
        SpeedUnit* unit;
        KPHUnit* kphUnit = new KPHUnit();
        MPHUnit* mphUnit = new MPHUnit();
};

#endif // SPEEDOMETER_H
