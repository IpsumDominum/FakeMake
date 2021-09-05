#include "threadPool.h"


void ThreadPool::threadJobsQueue()
{
    while(true)
    {   
        std::function<void()> Job;
        {
            std::unique_lock<std::mutex> lock(Queue_Mutex);
            //Wait until queue is not empty, or when queue should terminate
            condition.wait(lock, [this](){return !Queue.empty() || terminate_pool;});
            //Fetch the next node.
            Job = Queue.front();
            Queue.pop();
        }
        //Execute Job
        printDebug("DEBUG:: Executing Job\n");
        Job();
    }
};
void ThreadPool::AddJob(std::function<void()> New_Job)
{    
    //printDebug("DEBUG:: New Job Added.\n");
    std::unique_lock<std::mutex> lock(Queue_Mutex);
    Queue.push(New_Job);
    condition.notify_one();
}
void ThreadPool::terminate(){
    terminate_pool = true;
    terminate_pool = true; // use this flag in condition.wait
    condition.notify_all(); // wake up all threads.
    // Join all threads.
    for(std::thread &every_thread : pool)
    {   every_thread.join();}
    pool.clear();
    stopped = true;
}
ThreadPool::~ThreadPool(){
    if(!stopped){
        terminate();
    }
}
ThreadPool::ThreadPool(){    
    int numThreads = std::thread::hardware_concurrency();    
    for(int i = 0; i < numThreads; i++)    
    { 
        printDebug("DEBUG:: Thread Created\n");
        pool.push_back(std::thread(&ThreadPool::threadJobsQueue,this));}
    printDebug("DEBUG:: ThreadPool initiated with thread number "+std::to_string(numThreads)+"\n");
}
ThreadPool::ThreadPool(int numThreads){
    for(int i = 0; i < numThreads; i++)
    {  
        printDebug("Thread Created");
        pool.push_back(std::thread(&ThreadPool::threadJobsQueue,this));}
    printDebug("ThreadPool initiated with thread number "+std::to_string(numThreads));
}
