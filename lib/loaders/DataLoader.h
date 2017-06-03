#ifndef DATALOADER_H
#define DATALOADER_H

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <iostream>
#include <deque>
#include <queue>
#include <vector>
#include <thread>
#include <dirent.h>
#include <sys/types.h>

#include "PointsLoader.h"
#include "BoxLoader.h"

#include "../layouts/SubWindow.h"
#include "../utils/SafeQueue.h"
#include "../data/OXT.h"
#include "../data/BoxList.h"
#include "../data/CloudPoints.h"
#include "../data/ImageData.h"
#include "../objects/Gauge.h"

class DataLoader
{
    public:
        ~DataLoader();

        static DataLoader* getInstance();
        static const int QUEUE_SIZE = 20;
        static const int MIN_FILL = 10;

        void nextID();
        void update();
        void setFPS(int nfps);
        void setPlayingVideo(bool val);

        void attach(Observer<CloudPoints>*);
        void attach(Observer<OXT>*);
        void attach(Observer<BoxList>*);
        void attach(Observer<ImageData>*);

        void runWorker();
    protected:
    private:
        DataLoader();

        char path[100] = "/home/nghia/data/kitti";
        char date[20] = "2011_09_26";
        int drive = 36;

        int cnt = 0;
        int numImages = 0;          ///< Total number of frames in a video.
        int fps = 10;                ///< Frequency.
        bool isPlayingVideo = false;    ///< Flag indicating if broadcasting image id.
        bool isLoaded = false;          ///< Flag indicating if data is loaded.
        bool isStop = false;

        static DataLoader* mInstance;
        std::thread workerThread;

        std::vector<Observer<CloudPoints>*> cloudpointObservers;
        std::vector<Observer<ImageData>*> imageObservers;
        std::vector<Observer<BoxList>*> bboxObservers;
        std::vector<Observer<OXT>*> oxtObservers;

        SafeQueue<CloudPoints>* cloudpointQueue;
        SafeQueue<ImageData>* imageQueue;
        SafeQueue<BoxList>* bboxQueue;
        SafeQueue<OXT>* oxtQueue;

        void notify();

        void pop();

        void loadData(const char* id);
        void loadDataByThread(const char* id);

        int minQueueSize();

        static void* loadCloudpoints(SafeQueue<CloudPoints>*, std::string);
        static void* loadTexture(SafeQueue<ImageData>*, std::vector<std::string> filenames);
        static void* loadBBoxes(SafeQueue<BoxList>* queue, std::string filename);
        static void* loadOXT(SafeQueue<OXT>* queue, std::string filename);

        static void* runWorkerThread(DataLoader* dl, bool& isStop, int numImages, int startID);
};

#endif // DATALOADER_H
