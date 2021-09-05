#pragma once
#include "helper.h"
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <string>
#include "dependencyNode.h"
#include <functional>

class ThreadPool{
  public:
    ThreadPool(int numThreads);
    ThreadPool();
    ~ThreadPool();
    void AddJob(std::function<void()> New_Job);
    void terminate();
    void threadJobsQueue();
    bool isEmpty(){
        return Queue.size()==0;
    }
    std::vector<std::thread> pool;
    std::mutex Queue_Mutex;
    std::queue<std::function<void()>> Queue;
    std::condition_variable condition;
    bool terminate_pool = false;
    bool stopped = false;
};