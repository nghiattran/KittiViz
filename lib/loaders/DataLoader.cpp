#include "DataLoader.h"

DataLoader* DataLoader::mInstance = NULL;

DataLoader::DataLoader()
{
    char folderPath[400];
    sprintf(folderPath, "%s/%s/%s_drive_%04d_sync/velodyne_points/data", path, date, date, drive);

    DIR *dp = opendir(folderPath);
    if (dp == NULL) {
        printf("Wrong path: %s\n", folderPath);
        exit(1);
    }

    numImages = 0;
    struct dirent *dirp;
    while (dirp = readdir(dp)) {
        // If current or parent directory, skip it
        if(strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0 )
        {
            continue;
        }
        numImages++;
    }

    cloudpointQueue = new SafeQueue<CloudPoints>();
    imageQueue = new SafeQueue<ImageData>();
    bboxQueue = new SafeQueue<BoxList>();
    oxtQueue = new SafeQueue<OXT>();
}

DataLoader::~DataLoader()
{
    // Make sure worker thread is terminated.
    isStop = true;
    if (workerThread.joinable()) {
        workerThread.join();
    }

    // Deallocate memory
    delete cloudpointQueue;
    delete imageQueue;
    delete bboxQueue;
    delete oxtQueue;
}

/**
\brief Singleton constructor.

Create a singleton object of DataLoader.

*/

DataLoader* DataLoader::getInstance() {
    if (!mInstance)
    {
        mInstance = new DataLoader;
    }
    return mInstance;
}

void DataLoader::nextID() {
    if (isPlayingVideo)
        cnt = (cnt + 1) % (numImages * (60 / fps));

    if ((cnt % (60 / fps) == 0 && isPlayingVideo) || !isLoaded)
    {
        notify();
        isLoaded = true;
    }
}

/**
\brief Sets the boolean to play video or not.

\param b --- Draws the axes if true and not if false.

*/

void DataLoader::setPlayingVideo(bool val) {
    isPlayingVideo = val;
}

/**
\brief Sets frame rate.

\param nfps --- desired frame rate.

*/

void DataLoader::setFPS(int nfps) {
    fps = nfps;
}

/**
\brief Attach a BoxList listener to this object.

\param observer
*/

void DataLoader::attach(Observer<BoxList>* observer) {
    bboxObservers.push_back(observer);
}

/**
\brief Attach a CloudPoints listener to this object.

\param observer
*/

void DataLoader::attach(Observer<CloudPoints>* observer) {
    cloudpointObservers.push_back(observer);
}

/**
\brief Attach a ImageData listener to this object.

\param observer
*/

void DataLoader::attach(Observer<ImageData>* observer) {
    imageObservers.push_back(observer);
}

/**
\brief Attach a OXT listener to this object.

\param observer
*/

void DataLoader::attach(Observer<OXT>* observer) {
    oxtObservers.push_back(observer);
}

/**
\brief Perform pop on all queues.

*/

void DataLoader::pop() {
    if (cloudpointQueue->size() > 0) cloudpointQueue->pop();
    if (bboxQueue->size() > 0) bboxQueue->pop();
    if (imageQueue->size() > 0) imageQueue->pop();
    if (oxtQueue->size() > 0) oxtQueue->pop();
}

/**
\brief Notify all observers.

*/

void DataLoader::notify() {
    if (minQueueSize() == 0) return;

    BoxList boxes = bboxQueue->pop();
    for (int i = 0; i < bboxObservers.size(); i++) {
        bboxObservers[i]->update(boxes);
    }

    CloudPoints cp = cloudpointQueue->pop();
    for (int i = 0; i < cloudpointObservers.size(); i++) {
        cloudpointObservers[i]->update(cp);
    }

    ImageData imageData = imageQueue->pop();
    for (int i = 0; i < imageObservers.size(); i++) {
        imageObservers[i]->update(imageData);
    }

    OXT oxt = oxtQueue->pop();
    for (int i = 0; i < cloudpointObservers.size(); i++) {
        oxtObservers[i]->update(oxt);
    }
}

/**
\brief Load data without thread

*/

void DataLoader::loadData(const char* id) {
    // Load cloudpoints
    char filename0[400];
    sprintf(filename0, "%s/%s/%s_drive_%04d_sync/velodyne_points/data/%s.bin", path, date, date, drive, id);
    loadCloudpoints(cloudpointQueue, filename0);

    // Load tracklet
    char filename1[400];
    sprintf(filename1, "%s/%s/%s_drive_%04d_sync/tracklets/%s.txt", path, date, date, drive, id);
    loadBBoxes(bboxQueue, filename1);

    // Load images
    std::vector<std::string> filenames;
    for (int i = 0; i < 2; i++) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/image_%02d/data/%s.png", path, date, date, drive, i + 2, id);
        filenames.push_back(std::string (filename));
    }
    loadTexture(imageQueue, filenames);

    // Load oxts
    char filename3[400];
    sprintf(filename3, "%s/%s/%s_drive_%04d_sync/oxts/data/%s.txt", path, date, date, drive, id);
    loadOXT(oxtQueue, filename3);
}

/**
\brief Load data using thread

*/

void DataLoader::loadDataByThread(const char* id) {
    std::vector<std::thread*> t;

    // Load cloudpoints
    char filename0[400];
    sprintf(filename0, "%s/%s/%s_drive_%04d_sync/velodyne_points/data/%s.bin", path, date, date, drive, id);
    std::string newFilename0(filename0, 400);
    std::thread thread0 = std::thread(DataLoader::loadCloudpoints, std::ref(cloudpointQueue), newFilename0);
    t.push_back(&thread0);

    // Load tracklet
    char filename1[400];
    sprintf(filename1, "%s/%s/%s_drive_%04d_sync/tracklets/%s.txt", path, date, date, drive, id);
    std::string newFilename1(filename1, 400);
    std::thread thread1 = std::thread(DataLoader::loadBBoxes, std::ref(bboxQueue), newFilename1);
    t.push_back(&thread1);

    // Load images
    std::vector<std::string> filenames;
    for (int i = 0; i < 2; i++) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/image_%02d/data/%s.png", path, date, date, drive, i + 2, id);
        filenames.push_back(std::string(filename));
    }
    std::thread thread2 = std::thread(DataLoader::loadTexture, std::ref(imageQueue), filenames);
    t.push_back(&thread2);

    // Load oxts
    char filename3[400];
    sprintf(filename3, "%s/%s/%s_drive_%04d_sync/oxts/data/%s.txt", path, date, date, drive, id);
    std::string newFilename(filename3);
    std::thread thread3 = std::thread(DataLoader::loadOXT, std::ref(oxtQueue), filename3);
    t.push_back(&thread3);

    // Join threads
    for (int i = 0; i < t.size(); i++) {
        if (t[i]->joinable())
            t[i]->join();
    }
}

/**
\brief Threaded function to oxt;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadOXT(SafeQueue<OXT>* queue, std::string filename) {
    if (queue->size() == QUEUE_SIZE) {
        queue->pop();
    }
    queue->push(OXT(filename));
}


/**
\brief Threaded function to load cloudpoints;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadCloudpoints(SafeQueue<CloudPoints>* queue, std::string filename) {
    if (queue->size() == QUEUE_SIZE) {
        queue->pop();
    }
    queue->push(CloudPoints(filename));
}

/**
\brief Threaded function to load bboxes.

\param  queue - The queue to put data into.
\param  filename - name of tracklet file.
*/

void* DataLoader::loadBBoxes(SafeQueue<BoxList>* queue, std::string filename) {
    if (queue->size() == QUEUE_SIZE) {
        queue->pop();
    }
    queue->push(BoxList(filename));
}

/**
\brief Threaded function to load texture;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadTexture(SafeQueue<ImageData>* queue, std::vector<std::string> filenames)
{

    if (queue->size() == QUEUE_SIZE)
        queue->pop();

    queue->push(ImageData(filenames));
}

/**
\brief Get minimum number of item in all queues.

\return minSize - minimum items in all queue.
*/


int DataLoader::minQueueSize()
{
    int minSize = std::max(cloudpointQueue->size(), oxtQueue->size());
    minSize = std::min(minSize, bboxQueue->size());
    minSize = std::min(minSize, imageQueue->size());

    return minSize;
}

/**
\brief Public API for runThreadWorker function.
*/

void DataLoader::runWorker()
{
    if (!workerThread.joinable())
    {
        for (int i = 0; i < QUEUE_SIZE; i++)
        {
            char id[300];
            sprintf(id, "%010d", i);
            const char* constId = &id[0];\
            loadDataByThread(constId);\
        }

        DataLoader* itself = this;
        int num = QUEUE_SIZE;
        workerThread = std::thread(DataLoader::runWorkerThread, itself, std::ref(isStop), numImages, num);
    }
}

/**
\brief Run data loader worker thread which will fill all queues to reduce latency.

\param  dl - reference to main DataLoader object.
\param  isStop - reference boolean used to gracefully terminate thread.
\param  numImages - Maximum image id.
\param  startIndex - Start index of image id.
*/

void* DataLoader::runWorkerThread(DataLoader* dl, bool& isStop, int numImages, int startID)
{
    int cnt = startID;
    while (!isStop) {
        int minSize = dl->minQueueSize();
        if (minSize <= MIN_FILL)
        {
            char id[300];
            sprintf(id, "%010d", cnt);
            const char* constId = &id[0];
            dl->loadData(constId);
            cnt = (cnt + 1) % numImages;
        }
    }
}
