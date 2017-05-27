#include "CloudPoints.h"

CloudPoints::CloudPoints(const char *filename)
{
    loadData(filename);
}

CloudPoints::CloudPoints(std::string filename)
{
    loadData(filename.c_str());
}

CloudPoints::~CloudPoints()
{
    //dtor
}

void CloudPoints::loadData(const char *filename)
{
    std::ifstream fin(filename, std::ios::binary);
    if(!fin)
    {
        std::cout << " Error, Couldn't find file: " << filename << "\n";
        exit(1);
    }

    fin.seekg(0, std::ios::end);
    const size_t num_elements = fin.tellg() / sizeof(float);
    fin.seekg(0, std::ios::beg);

    std::vector<float> mdata(num_elements);
    fin.read(reinterpret_cast<char*>(&mdata[0]), num_elements*sizeof(float));

    data = mdata;
}

std::vector<float> CloudPoints::getData()
{
    return data;
}

float* CloudPoints::getArrayData()
{
    float arr[data.size()];
    for (int i = 0; i < data.size(); i++)
    {
        arr[i] = data[i];
    }
    return arr;
}
