#ifndef RULER_H
#define RULER_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "../objects/Models.h"
#include "../objects/Circle.h"
#include "../objects/TextRendererTTF.h"
#include "../utils/ProgramDefines.h"
#include "../utils/LoadShaders.h"
#include "../utils/Screen.h"

class Ruler
{
    public:
        Ruler(float mradius, float mgap, int mpts, float mfanAngle, Color mcolor, FadeStrategy mfadeStrategy)
        {
            screen = Screen::getInstance();

            radius = mradius;
            gap = mgap;
            color = mcolor;
            pts = mpts;
            fanAngle = mfanAngle;
            fadeStrategy = mfadeStrategy;

            glGenVertexArrays(1, &vboptr);
            glGenBuffers(1, &eboptr);
            glGenBuffers(1, &bufptr);

            LoadDataToGraphicsCard();
        };

        ~Ruler()
        {
            glBindVertexArray(vboptr);
            glDeleteBuffers(1, &bufptr);
            glDeleteBuffers(1, &eboptr);
            delete screen;
        }

        void loadTexts(glm::mat4 mlocation)
        {
            location = mlocation;

            textVector.clear();

            int numTexts = pts / SEPARATOR_RATE + 1;
            for (int i = 0; i < numTexts; i++)
            {
                TextRendererTTF text = TextRendererTTF("fonts/Tunga Bold.ttf");
                text.setFontSize(100);
                text.setColor(1, 1, 1, 1);

                textVector.push_back(text);
            }
        }

        void draw()
        {
            glBindVertexArray(vboptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
            glDrawElements(GL_TRIANGLES, pts * 3, GL_UNSIGNED_SHORT, NULL);

            // int height = screen->getHeight();
            // int width = screen->getWidth();
            // float screenRatio = (float) height / width;
            // float radius = 0.27;

            // for (int i = 0; i < textVector.size(); ++i)
            // {
            //     int reverseIndex = textVector.size() - 1 - i;
            //     std::string value = std::to_string(reverseIndex * SEPARATOR_RATE);
            //     int textWidth = textVector[i].textWidth(value);
            //     textWidth = 0;

            //     float phi = i * fanAngle / (textVector.size() - 1);
            //     float xpos = radius * cos(phi + 110*degf) * screenRatio;
            //     float ypos = radius * sin(phi + 110*degf);

            //     printf("%d %f\n", i, (phi + 110*degf) / PI *180);

            //     glm::mat4 textModel = location;
            //     textModel = glm::translate(textModel, glm::vec3(xpos - textWidth /1000, ypos, -0.1));
            //     // textModel = glm::rotate(textModel, 180*degf - (phi + 110*degf), glm::vec3(0, 0, 1));
            //     textModel = glm::rotate(textModel, phi - 75*degf, glm::vec3(0, 0, 1));
            //     textModel = glm::scale(textModel, glm::vec3(.0004));

            //     textVector[i].setModelMatrix(textModel);
            //     textVector[i].draw(value);
            // }
        }

        void setLightingAngle(float angle)
        {
            lightingAngle = angle;
            LoadDataToGraphicsCard();
        }

        float getFanAngle()
        {
            return fanAngle;
        }

    private:
        float radius;
        float gap;
        int pts;
        float fanAngle;
        float lightingAngle = 0;
        const int SEPARATOR_RATE = 20;
        glm::mat4 location;

        std::vector<TextRendererTTF> textVector = std::vector<TextRendererTTF>();

        Screen* screen;

        Color color;
        FadeStrategy fadeStrategy;

        GLuint vboptr;  ///< ID for faces VBO.
        GLuint eboptr;  ///< ID for faces index array.
        GLuint bufptr;  ///< ID for faces array buffer.

        void LoadDataToGraphicsCard()
        {
            GLuint vPosition = 0;
            GLuint vColor = 1;

            int num_pts = pts * 2;

            // Generate indices
            GLushort indices[pts * 3];
            for (int i = 0; i < pts / 2; i++)
            {
                int startIndex = i * 2 * 3;

                int zero = (i * 4 * 2) % num_pts;
                int one = (i * 4 * 2 + 1) % num_pts;
                int two = (i * 4 * 2 + 2) % num_pts;
                int three = (i * 4 * 2 + 3) % num_pts;

                indices[startIndex] = zero;
                indices[startIndex + 1] = one;
                indices[startIndex + 2] = two;

                indices[startIndex + 3] = one;
                indices[startIndex + 4] = two;
                indices[startIndex + 5] = three;
            }

            // Generate points
            GLfloat points[num_pts * 4];
            float r1 = radius;
            float r2 = r1 - gap;
            for (int i = 0; i < pts; i++)
            {
                float phi = i * fanAngle / (pts - 1);

                float nr2 = i % SEPARATOR_RATE == 0 || i % SEPARATOR_RATE == 1 ? r2 * 0.9 : r2;

                points[i * 8] = r1 * sin(phi);
                points[i * 8 + 1] = r1 * cos(phi);
                points[i * 8 + 2] = 0;
                points[i * 8 + 3] = 1;

                points[i * 8 + 4] = nr2 * sin(phi);
                points[i * 8 + 5] = nr2 * cos(phi);
                points[i * 8 + 6] = 0;
                points[i * 8 + 7] = 1;
            }

            GLfloat colors[num_pts * 4];
            for (int i = 0; i < num_pts; i++)
            {
                float phi = (i/2) * fanAngle / (pts - 1);
                float dimFactor = phi < lightingAngle ? 1 : 0.5;

                colors[i * 4] = color.red * dimFactor;
                colors[i * 4 + 1] = color.green * dimFactor;
                colors[i * 4 + 2] = color.blue * dimFactor;
                colors[i * 4 + 3] = fadeStrategy.getAlpha(i, pts);
            }

            glGenVertexArrays(1, &vboptr);
            glBindVertexArray(vboptr);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, bufptr);
            glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
            glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
            glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(sizeof(points)));
            glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

            glEnableVertexAttribArray(vPosition);
            glEnableVertexAttribArray(vColor);
        }
};

#endif // RULER_H
