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
std::atomic<bool> isProvided = false;

void signalHandler(int signal)
{
    std::cout << "\b\b";
    isFinishing = true;
}

void provider()
{
    while (!isFinishing.load())
    {
        {
            std::lock_guard<decltype(mtx)> lock(mtx);

            std::cout << "[PROVIDER] Is providing...\n";
            isProvided.store(true);
        }
        cv.notify_all();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "[PROVIDER] Stopping...\n";
    cv.notify_all();
}

void consumer()
{
    while (!isFinishing.load())
    {
        std::unique_lock<decltype(mtx)> lock(mtx);
        cv.wait(lock, [&]()
        {
            return isProvided.load() || isFinishing.load();
        });
        std::cout << "[CONSUMER] Fetched provider`s signal\n";
        isProvided.store(false);
    }

    std::cout << "[CONSUMER] Stopping...\n";
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