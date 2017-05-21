#ifndef GAUGE_H
#define GAUGE_H

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Models.h"
#include "../utils/ProgramDefines.h"
#include "../utils/LoadShaders.h"
#include "../utils/TextureController.h"
#include "../utils/Texture.h"

class Gauge : public Models {
  public:
      Gauge();
      ~Gauge();

      void setRadius(int r);
      void draw();
      void load();
      void LoadDataToGraphicsCard();
      void setModelMatrix(glm::vec3, glm::vec3, glm::vec3, float);
      void setScale(glm::vec3);
      void setTranformation(glm::vec3);
      void setRotation(glm::vec3, float);
  protected:
  private:
      const int NUM_PTS = 4;
      bool isLoaded = false;

      GLuint vboptr;  ///< ID for faces VBO.
      GLuint eboptr;  ///< ID for faces index array.
      GLuint bufptr;  ///< ID for faces array buffer.

      glm::mat4 model;
      glm::vec3 scale = glm::vec3(0.2);
      glm::vec3 transformation = glm::vec3(0);
      float rotAngle = 0;
      glm::vec3 rotVector = glm::vec3(0, 0, 1);

      TextureController* texCtrl = NULL;
      Texture* texture = NULL;
};

#endif // GAUGE_H
