#include "pch.h"
#include "foo.h"
#include <exception>

namespace UnitTests
{
    RTTI_DEFINITIONS(Foo);
    Foo::Foo(int value)
        : _data(new int(value))
    {
    }

    Foo::Foo(Foo&& other) noexcept
        : _data(other._data)
    {
        other._data = nullptr;
    }

    Foo::~Foo()
    {
        delete _data;
    }

    Foo::Foo(const Foo& other)
        : _data(new int(*other._data))
    {
    }

    Foo& Foo::operator=(const Foo& other)
    {
        if (this != &other)
        {
            *_data = *other._data;
        }

        return *this;
    }

    Foo& Foo::operator=(Foo&& other) noexcept
    {
        if (this != &other)
        {
            delete _data;
            _data = other._data;
            other._data = nullptr;
        }

        return *this;
    }

    bool Foo::operator==(const Foo& other) const
    {
        if (_data == nullptr || other._data == nullptr)
        {
            throw std::exception("_data and other_data should not be nullptr. Did you std::move() one of them?");
        }

        return *_data == *other._data;
    }

    bool Foo::operator!=(const Foo& other) const
    {
        return !(operator == (other));
    }


    const int& Foo::Data() const
    {
        if (_data == nullptr)
        {
            throw std::exception("_data should not be nullptr. Did you std::move() this instance?");
        }

        return *_data;
    }

    void Foo::SetData(int value)
    {
        *_data = value;
    }
}