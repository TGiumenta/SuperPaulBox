#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include <exception>
#include "Vector.h"
#include "DefaultHash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Foo>(const Foo& t)
    {
        RETURN_WIDE_STRING(t.Data());
    }

}

namespace UnitTestLibraryDesktop
{


    TEST_CLASS(DefaultHashTests)
    {
    public:

        TEST_CLASS_INITIALIZE(ClassInitialize)
        {

        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {

        }

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

        TEST_METHOD(TestDefaultHashInt)
        {
            DefaultHash<int> dh;
            size_t hash = dh(100);
            hash;

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(1);


            DefaultHash<Foo> dhFoo;
            size_t fooHashA = dhFoo(a);
            size_t fooHashD = dhFoo(d);
            size_t fooHashB = dhFoo(b);
            size_t fooHashC = dhFoo(c);
            size_t fooHashA2 = dhFoo(a);

            fooHashA;
            fooHashB;
            fooHashC;
            fooHashD;
            fooHashA2;
        }

        TEST_METHOD(TestCharPointer)
        {
            DefaultHash<const char*> dh;
            const char test[] = "test";
            const char test2[] = "test";

            size_t hash = dh(test);
            size_t hash2 = dh(test2);

            Assert::AreEqual(hash, hash2);

        }

        TEST_METHOD(TestString)
        {
            DefaultHash<std::string> dh;
            size_t hash = dh("test");
            size_t hash2 = dh("test");

            Assert::AreEqual(hash, hash2);
        }
        

    private:

        static _CrtMemState sStartMemState;


    };

    _CrtMemState DefaultHashTests::sStartMemState;
}
