#ifndef OXT_H
#define OXT_H

#include <iostream>
#include <string>

class OXT
{
    public:
        OXT(std::string filename);
        ~OXT();

        float lat, lon, alt, roll, pitch, yaw, vn, ve, vf, vl, vu, ax, ay, af, al, au, wx, wy, wz, wf, wl, wu, pos_accuracy, vel_accuracy;
        int navstat, numsats, posmode, velmode, orimode;
    protected:

    private:
};

#endif // OXT_H
