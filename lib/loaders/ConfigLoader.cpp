#include "ConfigLoader.h"

ConfigLoader* ConfigLoader::mInstance = NULL;

ConfigLoader::ConfigLoader()
{
    //ctor
}

ConfigLoader::~ConfigLoader()
{
    if (path)
        delete path;
    if (date)
        delete path;
}

ConfigLoader* ConfigLoader::getInstance()
{
    if (!mInstance)
    {
        mInstance = new ConfigLoader();
    }
    return mInstance;
}

void ConfigLoader::loadFile(std::string filename)
{
    isLoaded = true;

    std::ifstream infile(filename);
    if(!infile.is_open())
    {
        std::cout << " could not open file" << filename << std::endl;
        std::cout << " program terminating....\n";
        exit(EXIT_FAILURE);
    }

    std::string line;
    while(std::getline(infile, line))
    {
        std::istringstream is_line(line);
        std::string key;
        if(std::getline(is_line, key, '='))
        {
            std::string value;
            if(std::getline(is_line, value))
            {
                if (key == "path")
                {
                    if (path)
                        delete path;

                    path = strdup(value.c_str());
                } else if (key == "date")
                {
                    if (date)
                        delete path;

                    date = strdup(value.c_str());
                } else if (key == "drive")
                {
                    drive = std::stoi(value);
                }
            }
        }
    }
}

std::string ConfigLoader::getBasePathString(char folderPath[])
{
    getBasePath(folderPath);
    return std::string(folderPath);
}

void ConfigLoader::getBasePath(char folderPath[])
{
    if (!isLoaded)
    {
        printf("Unable to access base path because configuration is not loaded yet.\n");
        exit(1);
    }

    sprintf(folderPath, "%s/%s/%s_drive_%04d_sync", path, date, date, drive);
}

// void ConfigLoader::getVelodyneFile(char path[], int id)
// {
//     if (!isLoaded)
//     {
//         printf("Unable to access base path because configuration is not loaded yet.\n");
//         exit(1);
//     }

//     sprintf(folderPath, "%s/%s/%s_drive_%04d_sync", path, date, date, drive);
// }
