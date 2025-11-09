#include "some_class.h"

#include <string>
#include <stdexcept>
#include <limits>

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

bool MyClass::Count()
{
    if (counter_ == std::numeric_limits<decltype(counter_)>::max())
    {
        counter_ = 0;
        return false;
    }
    ++counter_;
    return true;
}