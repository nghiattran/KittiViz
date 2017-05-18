#ifndef DATALOADER_H
#define DATALOADER_H

#include <iostream>
#include <thread>

class DataLoaderWorker : public std::thread
{
    public:
        DataLoaderWorker();
        ~DataLoaderWorker();
    protected:
    private:
};

#endif // IMAGELOADER_H
