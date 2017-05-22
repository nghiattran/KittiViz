#include "OXT.h"
#include <sstream>
#include <string>
#include <fstream>

OXT::OXT(std::string filename)
{
    std::ifstream infile(filename);
    if(!infile.is_open())
    {
        std::cout << " could not open file" << filename << std::endl;
        std::cout << " program terminating....\n";
        exit(EXIT_FAILURE);
    }

    // Read these as float then cast to int later because of errors in KITTI data 
    float mnavstat, mnumsats, mposmode, mvelmode, morimode;

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        char objectType[10];
        if (!(iss >>lat >> lon >> alt >> roll >> pitch >> yaw >> vn >> ve >> vf >> vl >> vu >> ax >> ay >> ay >> af >> al >> au >> wx >> wy >> wz >> wf >> wl >> wu >> pos_accuracy >> vel_accuracy >> mnavstat >> mnumsats >> mposmode >> mvelmode >> morimode)) 
        {
            printf("Error occurs when parsing: %s\n", filename.c_str());
            exit(1); 
        }

        navstat = morimode;
        numsats = mnumsats;
        posmode = mposmode;
        velmode = mvelmode;
        orimode = morimode;
    }
}

OXT::~OXT()
{
    //dtor
}
