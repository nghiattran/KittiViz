#include "Gauge.h"

Gauge::Gauge()
{
}

Gauge::~Gauge() {
    if (texture)
        delete texture;

    if (texCtrl)
        delete texCtrl;
}

void Gauge::load() {
    isLoaded = true;

    texCtrl = TextureController::getInstance();
    sf::Image img = texCtrl->loadTextureFromFile("Textures/road1.jpg");
    texture = texCtrl->createNewTexture(img);

    texCtrl->useProgram();

    // setModelMatrix(scale, transformation, rotVector, rotAngle);
    setScale(glm::vec3(0.02));

    createTessellatedWallOBJ(2, 2, 2, 1);
    Models::load(0, 1, 2, 3);
    Models::setColor(1, 0, 0);

    LoadDataToGraphicsCard();
}

void Gauge::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;

    int num_pts = NUM_PTS * 2;

    // Generate indices
    GLushort indices[NUM_PTS * 3];
    for (int i = 0; i < NUM_PTS / 2 + 1; i++)
    {
        int startIndex = i * 2 * 3;

        int zero = (i * 2 * 2) % num_pts;
        int one = (i * 2 * 2 + 1) % num_pts;
        int two = (i * 2 * 2 + 2) % num_pts;
        int three = (i * 2 * 2 + 3) % num_pts;

        indices[startIndex] = zero;
        indices[startIndex + 1] = one;
        indices[startIndex + 2] = two;

        indices[startIndex + 3] = one;
        indices[startIndex + 4] = two;
        indices[startIndex + 5] = three;
    }

    // Generate points
    GLfloat points[num_pts * 4];
    float r1 = 1;
    float r2 = r1 - 0.2;
    for (int i = 0; i < NUM_PTS; i++)
    {
        float phi = 2 * PI / NUM_PTS * i;
        float y = 0;

        points[i * 8] = r1 * sin(phi);
        points[i * 8 + 1] = r1 * cos(phi);
        points[i * 8 + 2] = 0;
        points[i * 8 + 3] = 1;

        points[i * 8 + 4] = r2 * sin(phi);
        points[i * 8 + 5] = r2 * cos(phi);
        points[i * 8 + 6] = 0;
        points[i * 8 + 7] = 1;
    }

    GLfloat colors[num_pts * 3];
    for (int i = 0; i < num_pts; i++)
    {
        colors[i * 3] = 0;
        colors[i * 3 + 1] = float(i) / num_pts;
        colors[i * 3 + 2] = 0;
    }

    glGenVertexArrays(1, &vboptr);
    glBindVertexArray(vboptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

void Gauge::draw()
{
    // glLineWidth(1);
    // glBindVertexArray(vboptr);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    // glDrawElements(GL_TRIANGLE_STRIP, NUM_PTS * 2, GL_UNSIGNED_SHORT, NULL);
    // glDrawElements(GL_POINTS, NUM_PTS , GL_UNSIGNED_SHORT, NULL);


    texCtrl->turnTextureOn(texture);
    texCtrl->setModelMatrix(model);
    Models::draw();
    texCtrl->turnTextureOff(texture);
}

void Gauge::setModelMatrix(glm::vec3 s, glm::vec3 t, glm::vec3 rVec, float rAng)
{
    scale = s;
    transformation = t;
    rotVector = rVec;
    rotAngle = rAng;

    glm::mat4 model = glm::translate(glm::mat4(1.0), transformation);
    model = glm::rotate(model, rotAngle, rotVector);
    model = glm::scale(model, scale);

    texCtrl->setModelMatrix(model);
}

void Gauge::setScale(glm::vec3 s) {
    setModelMatrix(s, transformation, rotVector, rotAngle);
}

void Gauge::setTranformation(glm::vec3 t) {
    setModelMatrix(scale, t, rotVector, rotAngle);
}

void Gauge::setRotation(glm::vec3 rVec, float rAng) {
    setModelMatrix(scale, transformation, rVec, rAng);
}
