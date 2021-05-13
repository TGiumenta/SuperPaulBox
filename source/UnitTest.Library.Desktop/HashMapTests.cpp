#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include <exception>
#include "Vector.h"
#include "DefaultHash.h"
#include "HashMap.h"

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

namespace FieaGameEngine
{
    template<>
    class DefaultHash<const Foo>
    {
    public:
        std::size_t operator()(const Foo& key) const
        {
            return key.Data();
        }

    };
}


namespace UnitTestLibraryDesktop
{


    TEST_CLASS(HashMapTests)
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

        TEST_METHOD(TestConstructor)
        {
            HashMap<Foo, int> hashMap;
            const Foo a(1);
            const Foo b(2);

            hashMap.Insert(std::make_pair(a, 100));
            hashMap.Insert(std::make_pair(b, 420));
            hashMap.Insert(std::make_pair(a, 69));

            auto it = hashMap.begin();

        }

        TEST_METHOD(TestFind)
        {

        }

        TEST_METHOD(TestBrackets)
        {
            HashMap<const Foo, int> map;

            const Foo a(1);
            const Foo b(12);
            const Foo c(2);
            const Foo d(13);
            const Foo e(3);
            const Foo f(5);
            const Foo g(9);
            const Foo h(50);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            Assert::AreEqual(map.Size(), size_t(7));

            Assert::IsTrue(map[a] == 100);

            map[a] = 53;
            Assert::AreEqual(map.Size(), size_t(7));

            map[h] = 42;
            Assert::AreEqual(map.Size(), size_t(8));
            auto it = map.Find(h);
            Assert::AreEqual((*it).second, 42);
        }

        TEST_METHOD(TestRemove)
        {
            HashMap<const Foo, int> map;

            const Foo a(1);
            const Foo b(12);
            const Foo c(2);
            const Foo d(13);
            const Foo e(3);
            const Foo f(5);
            const Foo g(9);
            const Foo h(50);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            Assert::AreEqual(map.Size(), size_t(7));

            Assert::IsTrue(map.Remove(a));
            Assert::AreEqual(map.Size(), size_t(6));

            Assert::IsFalse(map.Remove(h));
            Assert::AreEqual(map.Size(), size_t(6));
        }

        TEST_METHOD(TestClear)
        {
            HashMap<const Foo, int> map;

            const Foo a(1);
            const Foo b(12);
            const Foo c(2);
            const Foo d(13);
            const Foo e(3);
            const Foo f(5);
            const Foo g(9);
            const Foo h(50);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            Assert::AreEqual(map.Size(), size_t(7));

            map.Clear();
            Assert::AreEqual(map.Size(), size_t(0));
        }

        TEST_METHOD(TestContainsKey)
        {
            HashMap<const Foo, int> map;

            const Foo a(1);
            const Foo b(12);
            const Foo c(2);
            const Foo d(13);
            const Foo e(3);
            const Foo f(5);
            const Foo g(9);
            const Foo h(50);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            Assert::IsTrue(map.ContainsKey(a));
            Assert::IsTrue(map.ContainsKey(d));
            Assert::IsTrue(map.ContainsKey(g));
            Assert::IsFalse(map.ContainsKey(h));

            map.Remove(d);
            Assert::IsFalse(map.ContainsKey(d));

        }

        TEST_METHOD(TestAt)
        {
            HashMap<const Foo, int> map;

            const Foo a(1);
            const Foo b(12);
            const Foo c(2);
            const Foo d(13);
            const Foo e(3);
            const Foo f(5);
            const Foo g(9);
            const Foo h(50);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            Assert::AreEqual(map.At(a), 100);
            Assert::AreEqual(map.At(d), 200);

            Assert::ExpectException<std::exception>([&map, &h] { map.At(h); });


            // Const version
            const HashMap<const Foo, int> constMap(map);
            Assert::AreEqual(constMap.At(a), 100);
            Assert::AreEqual(constMap.At(d), 200);

            Assert::ExpectException<std::exception>([&constMap, &h] { constMap.At(h); });
        }


    private:

        static _CrtMemState sStartMemState;


    };

    _CrtMemState HashMapTests::sStartMemState;
}
