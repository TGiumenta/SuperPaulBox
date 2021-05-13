#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include "Scope.h"
#include "Datum.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "Factory.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    std::wstring ToString<FooFactory>(const FooFactory* t)
    {
        RETURN_WIDE_STRING(t);
    }
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(FactoryTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
#ifdef _DEBUG
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
            _CrtMemCheckpoint(&sStartMemState);
#endif
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {

#ifdef _DEBUG
            _CrtMemState endMemState, diffMemState;
            _CrtMemCheckpoint(&endMemState);

            if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
            {
                _CrtMemDumpStatistics(&diffMemState);
                Assert::Fail(L"Memory Leaks!");
            }
#endif
        }

        TEST_METHOD(TestContructor)
        {
            
        }

        TEST_METHOD(TestAddRemove)
        {
            {
                Assert::AreEqual(Factory<RTTI>::Size(), size_t(0));
                const FooFactory fooFactory;
                Assert::AreEqual(Factory<RTTI>::Size(), size_t(1));
                Assert::IsTrue(Factory<Scope>::IsEmpty());

                Assert::ExpectException<std::exception>([] { const FooFactory anotherFooFactory; });
            }

            Assert::AreEqual(Factory<RTTI>::Size(), size_t(0));
            Assert::IsTrue(Factory<RTTI>::IsEmpty());
        }

        TEST_METHOD(TestCreate)
        {
            const FooFactory fooFactory;

            RTTI* newFoo = Factory<RTTI>::Create("Foo"); newFoo;
            Assert::IsTrue(newFoo->Is(Foo::TypeIdClass()));
            delete newFoo;
        }

        TEST_METHOD(TestFind)
        {
            const FooFactory fooFactory;
            const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo"s);
            Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));
        }

        


    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState FactoryTests::sStartMemState;
}
