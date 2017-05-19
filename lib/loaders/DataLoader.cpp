#include "DataLoader.h"

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

    cloudpointQueue = new SafeQueue <std::vector<float>>();
    bboxQueue = new SafeQueue <std::vector<BoundingBox>>();
}

DataLoader::~DataLoader()
{
    // Make sure worker thread is terminated.
    isStop = true;
    if (workerThread.joinable()) {
        workerThread.join();
    }

    delete cloudpointQueue;
    for (uint i = 0; i < textureQueues.size(); i++) {
        delete textureQueues[i];
    }
    delete bboxQueue;
}

/**
\brief Singleton constructor.

Create a singleton object of DataLoader.

*/

DataLoader* DataLoader::instance() {
    return new DataLoader();

    // TODO: FIX. something goes wrong with singleton
    // if (!s_instance)
    // {
    //     s_instance = new DataLoader();
    // }
    // return s_instance;
}

void DataLoader::nextID() {


    if (isPlayingVideo)
        cnt = (cnt + 1) % (numImages * (60 / fps));

    if ((cnt % (60 / fps) == 0 && isPlayingVideo) || !isLoaded)
    {

        // char id[300];
        // sprintf(id, "%010d", cnt / (60 / fps));
        // const char* constId = &id[0];

        // loadDataAndNotify(constId);

        // loadDataByThread(constId);

        notify();
    }

    isLoaded = true;
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
\brief Attach a CloudpointLoader to this object.

*/

void DataLoader::attachCloudpointLoader(PointsLoader* observer) {
    cloudpointObserver = observer;
}

/**
\brief Attach a BoxLoader to this object.

*/

void DataLoader::attachBoxLoader(BoxLoader* observer) {
    bboxObserver = observer;
}

/**
\brief Attach a TextureLoader to this object.

*/

void DataLoader::attachTextureLoader(SubWindow* observer) {
    textureObservers.push_back(observer);
    SafeQueue<sf::Image>* queue = new SafeQueue<sf::Image>();
    textureQueues.push_back(queue);
}

/**
\brief Perform pop on all queues.

*/

void DataLoader::pop() {
    if (cloudpointQueue->size() > 0) cloudpointQueue->pop();
    if (bboxQueue->size() > 0) bboxQueue->pop();

    for (uint i = 0; i < textureQueues.size(); i++) {
        if (textureQueues[i]->size() > 0) textureQueues[i]->pop();
    }
}

/**
\brief Notify all observers.

*/

void DataLoader::notify() {
    if (minQueueSize() == 0) return;

    int index = 0;
    if (cloudpointObserver)
        cloudpointObserver->LoadDataToGraphicsCard(cloudpointQueue->pop());

    if (bboxObserver)
        bboxObserver->loadBoxes(bboxQueue->pop());

    for (int i = 0; i < textureObservers.size(); i++) {
        textureObservers[i]->loadTexture(textureQueues[i]->pop());
    }
}

/**
\brief Load data without thread

*/

void DataLoader::loadData(const char* id) {
    if (cloudpointObserver) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/velodyne_points/data/%s.bin", path, date, date, drive, id);
        loadCloudpoints(cloudpointQueue, filename);
    }

    if (bboxObserver) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/tracklets/%s.txt", path, date, date, drive, id);
        loadBBoxes(bboxQueue, filename);
    }

    for (int i = 0; i < 2; i++) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/image_%02d/data/%s.png", path, date, date, drive, i + 2, id);
        loadTexture(textureQueues[i], filename);
    }
}

/**
\brief Load data using thread

*/

void DataLoader::loadDataByThread(const char* id) {
    int startIndex = cloudpointObserver ? 1 : 0;
    startIndex += bboxQueue ? 1 : 0;

    int numThreads = textureQueues.size() + startIndex;
    std::thread t[numThreads];

    if (cloudpointObserver) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/velodyne_points/data/%s.bin", path, date, date, drive, id);
        std::string newFilename(filename, 400);
        t[0] = std::thread(DataLoader::loadCloudpoints, std::ref(cloudpointQueue), newFilename);
    }

    if (bboxObserver) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/tracklets/%s.txt", path, date, date, drive, id);
        std::string newFilename(filename, 400);
        t[cloudpointObserver ? 1 : 0] = std::thread(DataLoader::loadBBoxes, std::ref(bboxQueue), newFilename);
    }

    for (int i = 0; i < textureQueues.size(); i++) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/image_%02d/data/%s.png", path, date, date, drive, i + 2, id);
        std::string newFilename(filename);

        t[startIndex + i] = std::thread(DataLoader::loadTexture, std::ref(textureQueues[i]), newFilename);
    }

    for (int i = 0; i < numThreads; i++) {
        if (t[i].joinable())
            t[i].join();
    }
}

/**
\brief Threaded function to load Cloudpoints;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadCloudpoints(SafeQueue<std::vector<float>>* queue, std::string filename) {
    if (queue->size() == QUEUE_SIZE) {
        queue->pop();
    }
    std::vector<float> data = DataLoader::readBin(filename.c_str());

    queue->push(data);
}

/**
\brief Threaded function to load bboxes.

\param  queue - The queue to put data into.
\param  filename - name of tracklet file.
*/

void* DataLoader::loadBBoxes(SafeQueue<std::vector<BoundingBox>>* queue, std::string filename) {

    if (queue->size() == QUEUE_SIZE) {
        queue->pop();
    }

    std::ifstream infile(filename);
    if(!infile.is_open())
    {
        std::cout << " could not open file" << filename << std::endl;
        std::cout << " program Terminating....\n";
        exit(EXIT_FAILURE);
    }

    std::vector<BoundingBox> data = std::vector<BoundingBox>();
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        char objectType[10];
        float h, w, l, tx, ty, tz, rx, ry, rz;
        if (!(iss >> objectType >> h >> w >> l >> tx >> ty >> tz >> rx >> ry >> rz)) { exit(1); } // error

        glm::vec3 size = glm::vec3(l, h, w);
        glm::vec3 transform = glm::vec3(tx, ty, tz);
        glm::vec3 rotation = glm::vec3(rx, ry, rz);

        data.push_back(BoundingBox(objectType, size, transform, rotation));
    }

    queue->push(data);
}

/**
\brief Threaded function to loadCloudpoints;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadTexture(SafeQueue<sf::Image>* queue, std::string filename) {

    if (queue->size() == QUEUE_SIZE)
        queue->pop();

    sf::Image texture;
    bool texloaded = texture.loadFromFile(filename);

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }

    queue->push(texture);
}

/**
\brief Load data and notify;

*/

void DataLoader::loadDataAndNotify(const char* id) {
    int startIndex = cloudpointObserver ? 1 : 0;
    int numThreads = textureQueues.size() + startIndex;

    std::thread t[numThreads];

    if (cloudpointObserver) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/velodyne_points/data/%s.bin", path, date, date, drive, id);
        std::string newFilename(filename);
        t[0] = std::thread(DataLoader::loadCloudpointsAndNotify, std::ref(cloudpointObserver), newFilename);
    }

    for (int i = 0; i < textureObservers.size(); i++) {
        char filename[400];
        sprintf(filename, "%s/%s/%s_drive_%04d_sync/image_%02d/data/%s.png", path, date, date, drive, i + 2, id);
        std::string newFilename(filename);

        t[startIndex + i] = std::thread(DataLoader::loadTextureAndNotify, std::ref(textureObservers[i]), newFilename);
    }

    for (int i = 0; i < numThreads; i++) {
        if (t[i].joinable()) {
            t[i].join();
        }
    }
}

/**
\brief Threaded function to load cloudpoints and notify observer;

\param  pl - PointLoader object to load data onto.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadCloudpointsAndNotify(PointsLoader* pl,
                                  std::string filename)
{

    std::vector<float> data = DataLoader::readBin(filename.c_str());
    pl->LoadDataToGraphicsCard(data);
}

/**
\brief Threaded function to load cloudpoints;

\param  queue - The queue to put data into.
\param  filename - name of cloudpoint file.
*/

void* DataLoader::loadTextureAndNotify(SubWindow* sw,
                              std::string filename)
{
    sf::Image texture;
    bool texloaded = texture.loadFromFile(filename);

    if (!texloaded)
    {
        std::cerr << "Could not load texture." << std::endl;
        exit(EXIT_FAILURE);
    }
    sw->loadTexture(texture);
}


/**
\brief Load binary file and put data in float vector.

\param  filename - name of binary file.
 */

std::vector<float> DataLoader::readBin(const char *filename) {
    std::ifstream fin(filename, std::ios::binary);
    if(!fin)
    {
        std::cout << " Error, Couldn't find file: " << filename << "\n";
        exit(1);
    }

    fin.seekg(0, std::ios::end);
    const size_t num_elements = fin.tellg() / sizeof(float);
    fin.seekg(0, std::ios::beg);

    std::vector<float> data(num_elements);
    fin.read(reinterpret_cast<char*>(&data[0]), num_elements*sizeof(float));

    return data;
}

/**
\brief Get minimum number of item in all queues.

\return minSize - minimum items in all queue.
*/


int DataLoader::minQueueSize()
{
    int minSize = std::max((int) cloudpointQueue->size(), 0);
    for (int i = 0; i < textureQueues.size(); i++)
    {
        minSize = std::min(minSize, (int) textureQueues[i]->size());
    }
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
            const char* constId = &id[0];
            loadDataByThread(constId);
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

/**
\brief Clear all boxes.
*/

void DataLoader::clearBoxes(std::vector<BoundingBox> boxes)
{
    // for (std::vector<BoundingBox*>::iterator it = boxes.begin() ; it != boxes.end(); ++it)
    // {
    //     delete (*it);
    // }
    boxes.clear();
}
