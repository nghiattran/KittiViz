#ifndef SCREEN_H
#define SCREEN_H

#include <cstddef>
#include <stdio.h>

class Screen
{
    public:
        static Screen* getInstance();
        virtual ~Screen();

        void update(int height, int width);
        int getHeight() const;
        int getWidth() const;
    protected:
    private:
        Screen();

        static Screen* mInstance;

        int height;
        int width;
};

#endif // SCREEN_H
