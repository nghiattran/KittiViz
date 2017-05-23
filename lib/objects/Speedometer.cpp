#include "Speedometer.h"

Speedometer::Speedometer() : Gauge(UintColor(44, 154,199))
{
    unit = mphUnit;
    unit->setLocationMatrix(location);
}

Speedometer::~Speedometer()
{
    delete mphUnit;
    delete kphUnit;
}

void Speedometer::draw()
{
    Gauge::draw();

    setModelMatrix(levels[2]);
    unit->draw();
}

void Speedometer::update(OXT oxt)
{
    float value = sqrt(pow(oxt.vf, 2) + pow(oxt.vl, 2) + pow(oxt.vu, 2));
    unit->updateSpeed(value);
}

float Speedometer::getSpeed()
{
    return unit->getSpeed();
}

void Speedometer::toggleUnit()
{
    float speed = unit->reverse(getSpeed());

    if (unit == mphUnit) {
        unit = kphUnit;
    } else {
        unit = mphUnit;
    }

    unit->setLocationMatrix(location);
    unit->updateSpeed(speed);
}
