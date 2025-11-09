#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <csignal>

#include "some_class.h"

MyClass* someData = new MyClass();

std::mutex mtx; 
std::condition_variable cv;
std::atomic<bool> isFinishing = false;
std::atomic<bool> isProvided = false;

void signalHandler(int signal)
{
    std::cout << "\b\bEXIT SIGNAL CAUGHT\n";
    isFinishing = true;
}

void provider()
{
    while (!isFinishing.load()) ///< Check for exit flag
    {
        {
            std::lock_guard<decltype(mtx)> lock(mtx); ///< Locking our mutex

            std::cout << "[PROVIDER] Is providing...\n"; 
            someData->Count(); ///< Doing some job
            isProvided.store(true); ///< Setting flag that the job is completed
        } ///< Our mutex will be freed here 
        cv.notify_one(); ///< Here we say to consumer that "Hey, you can receive my work result, i`ve finished"
        std::this_thread::sleep_for(std::chrono::seconds(1)); ///< Sleeping, because tired idk
    }

    std::cout << "[PROVIDER] Stopping...\n"; ///< We will come here only if isFinished
    cv.notify_all(); ///< We must call to all consumers that they must stop waiting
}

void consumer(int id)
{
    while (!isFinishing.load()) ///< Same, just work until user says not to
    {
        std::unique_lock<decltype(mtx)> lock(mtx); ///< Here we receive out locked mutex
        cv.wait(lock, [&]()
        {
            return isProvided.load() || isFinishing.load();
        }); ///< The most interesting part - here we free the thread until we get cv notication AND lambda returns true

        if (isFinishing.load())
        {
            break; ///< Check to exit if we get notification to exit and not to receive processed data
        }

        std::cout << "[CONSUMER" + std::to_string(id) + "] Fetched provider`s signal: " + std::to_string(someData->GetCounter()) + "\n"; ///< Just logging our id and data
        isProvided.store(false);
    }

    std::cout << "[CONSUMER" + std::to_string(id) + "] Stopping...\n"; ///< Logging that we are stopping
}

int main()
{
    signal(SIGTERM, signalHandler); ///< Reusing system signals to call our function of setting exit flag
    signal(SIGINT, signalHandler);

    std::thread providerThread(provider); ///< Starting provider thread

    std::thread consumerThread1(consumer, 1); ///< Starting consumer threads 
    std::thread consumerThread2(consumer, 2);

    providerThread.join(); ///< Waiting for provider thread to stop

    consumerThread1.join(); ///< Waiting for consumer threads to stop
    consumerThread2.join();

    std::cout << "Execution completed\n";
    return EXIT_SUCCESS; ///< We will always come here on ctrl + c
}