#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "some_class.h"

MyClass someData;

std::mutex mtx; 
std::condition_variable cv;
std::atomic<bool> isFinishing = false;

void provider()
{
    std::lock_guard<decltype(mtx)> lock(mtx);
    
    while(!isFinishing)
    {
        std::cout << "Provider is waiting\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cv.notify_all();
}

void consumer()
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []()
    {
        return !isFinishing;
    });

    std::cout << "Consumer stopping\n";
}

int main()
{
    std::thread providerThread(provider);
    std::thread consumerThread(consumer);

    providerThread.join();
    consumerThread.join();

    std::cout << "Execution completed\n";
    return EXIT_SUCCESS;
}