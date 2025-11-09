#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <csignal>

#include "some_class.h"

MyClass someData;

std::mutex mtx; 
std::condition_variable cv;
std::atomic<bool> isFinishing = false;

void signalHandler(int signal)
{
    std::cout << "\b\b";
    isFinishing = true;
}

void provider()
{
    std::lock_guard<decltype(mtx)> lock(mtx);
    
    while(!isFinishing.load())
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
        return isFinishing.load();
    });

    std::cout << "Consumer stopping\n";
}

int main()
{
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    std::thread providerThread(provider);
    std::thread consumerThread(consumer);

    providerThread.join();
    consumerThread.join();

    std::cout << "Execution completed\n";
    return EXIT_SUCCESS;
}