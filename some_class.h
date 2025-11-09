#pragma once

#include <cstdint>
#include <memory>

class MyClass
{
public:
    MyClass();
    MyClass(const std::string& name, size_t length);

    ~MyClass();

    bool Count();

private:
    char* string_;
    size_t stringSize_;

    uint64_t counter_;
};