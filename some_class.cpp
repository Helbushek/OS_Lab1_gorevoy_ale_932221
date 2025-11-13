#include "some_class.h"

#include <string>
#include <stdexcept>
#include <limits>

MyClass::MyClass()
    : string_("Not initialized string, or something")
    , counter_(0)
{}

MyClass::MyClass(const std::string& name, size_t length)
{
    if (name.length() > length)
    {
        throw std::logic_error("Name`s length must be equal to length");
    }

    string_ = name;
}

bool MyClass::Count()
{
    if (counter_ == std::numeric_limits<decltype(counter_)>::max())
    {
        counter_ = 0;
        return false;
    }
    string_ = (counter_ % 2 == 0 ? "A" : "B");

    ++counter_;
    return true;
}

int MyClass::GetCounter()
{
    return counter_;
}

std::string MyClass::GetString()
{
    return string_;
}