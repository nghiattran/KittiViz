#ifndef BOXLIST_H
#define BOXLIST_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

#include "../data/BoundingBox.h"

class BoxList
{
    public:
        BoxList(std::string filename);
        virtual ~BoxList();
        std::vector<BoundingBox> getData();
    protected:

    private:
        std::vector<BoundingBox> data;
};

#endif // BOXLIST_H
