#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Models.h"
#include "../objects/Circle.h"
#include "../objects/TextRendererTTF.h"
#include "../utils/ProgramDefines.h"
#include "../utils/LoadShaders.h"


struct Color
{
    float red, green, blue;
    Color(float r = 0, float g = 0, float b = 0): red(r), green(g), blue(b) {}
};

struct UintColor : public Color
{
    UintColor(uint r = 0, uint g = 0, uint b = 0): Color(r/255.0, g/255.0, b/255.0) {}
};


struct FadeStrategy
{
    FadeStrategy() {};
    virtual float getAlpha(int index, int numPts) {
        return 1.0;
    };
};

struct OneEndFade : public FadeStrategy
{
    float step, start;
    OneEndFade(float mstep, float mstart) : step(mstep), start(mstart) {
    }
    virtual float getAlpha(int index, int numPts) override {
        return step * index / numPts + start;
    };
};

struct InnerFade : public FadeStrategy
{
    InnerFade() {}
    virtual float getAlpha(int index, int numPts) override {
        return index % 2 == 0 ? 1.0f : 0.0f;
    };
};

struct CircleData
{
    const GLuint vboptr, eboptr;
    CircleData(GLuint& v, GLuint& e): vboptr(v), eboptr(e) {}
};

class Circle
{
    public:
        Circle(float mradius, float mgap, int mpts, float mfanAngle, Color mcolor, FadeStrategy* mfadeStrategy)
        {
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

        ~Circle()
        {
            glBindVertexArray(vboptr);
            glDeleteBuffers(1, &bufptr);
            glDeleteBuffers(1, &eboptr);

            // TODO: fix delete shared pointer 
            // if (fadeStrategy)
            //     delete fadeStrategy;
        }

        void setFanAngle(float mfanAngle)
        {
            fanAngle = mfanAngle;
            LoadDataToGraphicsCard();
        }

        void draw()
        {
            glBindVertexArray(vboptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
            glDrawElements(GL_TRIANGLE_STRIP, pts * 3, GL_UNSIGNED_SHORT, NULL);
        }
    private:
        float radius;
        float gap;
        Color color;
        int pts;
        float fanAngle;

        FadeStrategy* fadeStrategy;
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
            float r1 = radius;
            float r2 = r1 - gap;
            for (int i = 0; i < pts; i++)
            {
                float phi = i * fanAngle / (pts - 1);

                points[i * 8] = r1 * sin(phi);
                points[i * 8 + 1] = r1 * cos(phi);
                points[i * 8 + 2] = 0;
                points[i * 8 + 3] = 1;

                points[i * 8 + 4] = r2 * sin(phi);
                points[i * 8 + 5] = r2 * cos(phi);
                points[i * 8 + 6] = 0;
                points[i * 8 + 7] = 1;
            }

            GLfloat colors[num_pts * 4];
            for (int i = 0; i < num_pts; i++)
            {
                colors[i * 4] = color.red;
                colors[i * 4 + 1] = color.green;
                colors[i * 4 + 2] = color.blue;
                colors[i * 4 + 3] = fadeStrategy->getAlpha(i, pts);
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

#endif // CIRCLE_H
