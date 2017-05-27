#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

class ImageData
{
    public:
        ImageData(std::vector<std::string> filenames);
        ~ImageData();

        std::vector<sf::Image> getData() const;
    protected:

    private:
      std::vector<sf::Image> data;
};

#endif // IMAGEDATA_H
