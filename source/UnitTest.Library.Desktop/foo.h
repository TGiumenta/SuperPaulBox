#pragma once
#include "RTTI.h"
#include "Factory.h"



namespace UnitTests
{
    class Foo : public FieaGameEngine::RTTI
    {
        RTTI_DECLARATIONS(Foo, RTTI);
    public:
        explicit Foo(int value = 0);

        Foo(const Foo& other);
        Foo(Foo&& other) noexcept;

        ~Foo();

        Foo& operator=(const Foo& other);
        Foo& operator=(Foo&& other) noexcept;

        bool operator==(const Foo& other) const;
        bool operator!=(const Foo& other) const;

        const int& Data() const;
        void SetData(int value);

    private:
        int* _data;
    };

    using namespace FieaGameEngine;
    ConcreteFactory(Foo, RTTI);
}