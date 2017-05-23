#include "Screen.h"

Screen* Screen::mInstance = NULL;

Screen::Screen()
{
    //ctor
}

Screen::~Screen()
{
    //dtor
}


Screen* Screen::getInstance()
{
    if (!mInstance)
    {
        mInstance = new Screen();
    }
    return mInstance;
}

void Screen::update(int mheight, int mwidth)
{
    height = mheight;
    width = mwidth;
}

int Screen::getHeight() const
{
    return height;
}

int Screen::getWidth() const
{
    return width;
}
