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
#include <sstream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

#include "PointsLoader.h"
#include "BoxLoader.h"

#include "../layouts/SubWindow.h"
#include "../utils/SafeQueue.h"

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
        void attachCloudpointLoader(PointsLoader*);
        void attachTextureLoader(SubWindow*);
        void attachBoxLoader(BoxLoader*);
        void runWorker();
    protected:
    private:
        DataLoader();

        char path[100] = "/home/nghia/workplace/data/kitti";
        char date[20] = "2011_09_26";
        int drive = 1;

        int cnt = 0;
        int numImages = 802;          ///< Total number of frames in a video.
        int fps = 10;                ///< Frequency.
        bool isPlayingVideo = false;    ///< Flag indicating if broadcasting image id.
        bool isLoaded = false;          ///< Flag indicating if data is loaded.
        bool isStop = false;

        static DataLoader* mInstance;
        std::thread workerThread;

        SafeQueue <std::vector<float>>* cloudpointQueue;
        PointsLoader* cloudpointObserver = NULL;

        SafeQueue <std::vector<BoundingBox>>* bboxQueue;
        BoxLoader* bboxObserver = NULL;

        std::vector <SafeQueue<sf::Image>*> textureQueues;
        std::vector <SubWindow*> textureObservers;

        std::queue<std::thread> threadQueue;

        void notify();

        void pop();

        void loadData(const char* id);
        void loadDataByThread(const char* id);

        int minQueueSize();
        int minQueueSize(DataLoader* dl);

        static void* loadCloudpoints(SafeQueue<std::vector<float>>*, std::string);
        static void* loadTexture(SafeQueue<sf::Image>*, std::string);
        static void* loadBBoxes(SafeQueue<std::vector<BoundingBox>>* queue, std::string filename);

        void loadDataAndNotify(const char* id);
        static void* loadCloudpointsAndNotify(PointsLoader*, std::string);
        static void* loadTextureAndNotify(SubWindow*, std::string);

        static std::vector<float> readBin(const char *filename);   ///< Load binary vector from bin file.
        static void* runWorkerThread(DataLoader* dl, bool& isStop, int numImages, int startID);

        void clearBoxes(std::vector<BoundingBox>);
};

#endif // DATALOADER_H
