#pragma once

#include <cstdint>
#include <string>

class MyClass
{
public:
    MyClass();
    MyClass(const std::string& name, size_t length);

    ~MyClass() = default;

    bool Count();

    int GetCounter();

    std::string GetString();

private:
    std::string string_;

    uint64_t counter_;
};