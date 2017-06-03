#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <string.h>

class ConfigLoader
{
    public:
        virtual ~ConfigLoader();

        static ConfigLoader* getInstance();

        void loadFile(std::string);
        void getBasePath(char folderPath[]);
        std::string getBasePathString(char folderPath[]);
    protected:
    private:
        ConfigLoader();

        static ConfigLoader* mInstance;

        bool isLoaded = false;
        char* path = NULL;
        char* date = NULL;
        int drive = -1;
};

#endif // CONFIGLOADER_H
