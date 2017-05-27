#include "BoxList.h"

BoxList::BoxList(std::string filename)
{
    std::ifstream infile(filename);
    if(!infile.is_open())
    {
        std::cout << " could not open file" << filename << std::endl;
        std::cout << " program Terminating....\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        char objectType[10];
        float h, w, l, tx, ty, tz, rx, ry, rz;
        if (!(iss >> objectType >> h >> w >> l >> tx >> ty >> tz >> rx >> ry >> rz)) { exit(1); } // error

        glm::vec3 size = glm::vec3(l, h, w);
        glm::vec3 transform = glm::vec3(tx, ty, tz);
        glm::vec3 rotation = glm::vec3(rx, ry, rz);

        data.push_back(BoundingBox(objectType, size, transform, rotation));
    }
}

BoxList::~BoxList()
{
    //dtor
}

std::vector<BoundingBox> BoxList::getData()
{
    return data;
}