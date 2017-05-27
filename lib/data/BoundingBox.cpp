#include "BoundingBox.h"

BoundingBox::BoundingBox(char type[], glm::vec3 s, glm::vec3 t, float r)
{
    setData(type, s, t, r);
}

BoundingBox::BoundingBox(char type[], glm::vec3 s, glm::vec3 t, glm::vec3 r)
{
    setData(type, s, t, r.z);
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::setData(char type[], glm::vec3 s, glm::vec3 t, float r)
{
    objectType = type;
    transform = glm::vec3(t.x, t.z, -t.y);
    size = s;
    rotAngle = r;

    model = glm::mat4(1.0);
    // Add vec3 because the tranformation is based on the center of bottom triangle
    model = glm::translate(model, transform + glm::vec3(0, size.y / 2, 0));
    model = glm::rotate(model, rotAngle, glm::vec3(0, 1, 0));
    model = glm::scale(model, size);
}

glm::mat4 BoundingBox::getModelMatrix()
{
    return model;
}
