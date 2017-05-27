#ifndef CAMERAIMAGE_H
#define CAMERAIMAGE_H

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../objects/Models.h"
#include "../utils/TextureController.h"

class CameraImage : public TextureController, Models
{
    public:
        CameraImage();
        ~CameraImage();

        void draw();
        const glm::mat4 getModelMatrix();
        void setModelMatrix(glm::mat4 nmodel) override;
        void setSize(float h, float w);
    protected:
    private:

        void update(const char * data);       ///< Implementation of observer pattern.
};

#endif // CAMERAIMAGE_H
