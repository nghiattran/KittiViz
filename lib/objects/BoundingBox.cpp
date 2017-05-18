#include "BoundingBox.h"


BoundingBox::BoundingBox(char type[], glm::vec3 s, glm::vec3 t, float r)
{
    objectType = type;
    transform = glm::vec3(t.x - 1, t.z, -t.y - 2);
    size = s;
    rotAngle = r;
}

BoundingBox::BoundingBox(char type[], glm::vec3 s, glm::vec3 t, glm::vec3 r)
{
    printf("%s %f %f %f %f\n", type, t.x, t.y, t.z, r.z);
    objectType = type;
    transform = glm::vec3(t.x, t.z, -t.y);
    size = s;
    rotAngle = r.z;
}

BoundingBox::~BoundingBox()
{
}

glm::mat4 BoundingBox::getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, transform + glm::vec3(0, size.y / 2, 0));
    model = glm::rotate(model, (float) (1 * PI / 2) + rotAngle, glm::vec3(0, 1, 0));
    model = glm::scale(model, size);
    return model;
}
