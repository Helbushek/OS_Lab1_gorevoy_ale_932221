#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <stdexcept>

class MyClass
{
public:
    MyClass();
    MyClass(const std::string& name, size_t length);

    ~MyClass();

private:
    char* string_;
    size_t stringSize_;
};

MyClass::MyClass()
    : string_(nullptr)
    , stringSize_(0)
{}

MyClass::MyClass(const std::string& name, size_t length)
{
    if (name.length() > length)
    {
        throw std::logic_error("Name`s length must be equal to length");
    }

    string_ = new char[length];

    for (size_t i = 0; i < length; ++i)
    {
        if (name[i] == '\0')
        {
            return;
        }
        string_[i] = name[i];
    }
}

MyClass::~MyClass()
{
    if (string_)
    {
        delete string_;
    }
}