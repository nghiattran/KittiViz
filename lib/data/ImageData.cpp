#include "ImageData.h"

ImageData::ImageData(std::vector<std::string> filenames)
{
    for (int i = 0; i < filenames.size(); i++)
    {
        sf::Image texture;
        bool texloaded = texture.loadFromFile(filenames[i]);

        if (!texloaded)
        {
            std::cerr << "Could not load texture." << std::endl;
            exit(EXIT_FAILURE);
        }
        data.push_back(texture);
    }
}

ImageData::~ImageData()
{
    //dtor
}

std::vector<sf::Image> ImageData::getData() const
{
    return data;
}
