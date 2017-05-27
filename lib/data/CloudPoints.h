#ifndef CLOUDPOINTS_H
#define CLOUDPOINTS_H

#include <vector>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

class CloudPoints
{
    public:
        CloudPoints(const char *filename);
        CloudPoints(std::string filename);
        ~CloudPoints();

        float* getArrayData();
        std::vector<float> getData();
    protected:

    private:
        std::vector<float> data;
        void loadData(const char *filename);

};

#endif // CLOUDPOINTS_H
