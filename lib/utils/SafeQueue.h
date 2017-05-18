// This class is taken from https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/

#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <typename T>
class SafeQueue
{
    public:
        SafeQueue(){}
        SafeQueue(const SafeQueue<T> &obj) {
            // This copy constructor is only to prevent default contructor from being used.
            throw std::invalid_argument("SafeQueue does not support copying.");
        }

        T pop()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                cond_.wait(mlock);
            }
            auto item = queue_.front();
            queue_.pop();
            return item;
        }
       
        void pop(T& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            while (queue_.empty())
            {
                cond_.wait(mlock);
            }
            item = queue_.front();
            queue_.pop();
        }
       
        void push(const T& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push(item);
            mlock.unlock();
            cond_.notify_one();
        }
       
        void push(T&& item)
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            queue_.push(std::move(item));
            mlock.unlock();
            cond_.notify_one();
        }

        int size()
        {
            std::unique_lock<std::mutex> mlock(mutex_);
            int size = queue_.size();
            mlock.unlock();
            cond_.notify_one();
            return size;
        }
       
    private:
        std::queue<T> queue_;
        mutable std::mutex mutex_;
        std::condition_variable cond_;
};
#endif