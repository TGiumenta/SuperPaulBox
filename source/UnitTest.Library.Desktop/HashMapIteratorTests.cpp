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
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<Foo>(const Foo& t)
    {
        RETURN_WIDE_STRING(t.Data());
    }

    template<>
    inline std::wstring ToString<HashMap<Foo, int>::PairType>(const HashMap<Foo, int>::PairType& t)
    {
        try
        {
            std::wstringstream s;	
            s << L"("s << ToString(t.first) << L", "s << t.second << L")"s;
            return s.str();
        }
        catch (...)
        {
            return L"end()";
        }
    }

    template<>
    inline std::wstring ToString<HashMap<const Foo, int>::Iterator>(const HashMap<const Foo, int>::Iterator& t)
    {
        try
        {
            return ToString(*t);
        }
        catch (...)
        {
            return L"end()";
        }
    }

    template<>
    inline std::wstring ToString<HashMap<const Foo, int>::ConstIterator>(const HashMap<const Foo, int>::ConstIterator& t)
    {
        try
        {
            return ToString(*t);
        }
        catch (...)
        {
            return L"end()";
        }
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


    TEST_CLASS(HashMapIteratorTests)
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
            // const stuff
            HashMap<const Foo, int> constMap;
            const Foo a(1);
            const Foo b(12);

            constMap.Insert(std::make_pair(a, 100));
            constMap.Insert(std::make_pair(b, 100));

            auto constIt = constMap.begin();
            ++constIt;
        }

        TEST_METHOD(TestBegin)
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

            auto beginIt = map.begin();
            auto endIt = map.end();
            Assert::AreEqual(beginIt, endIt);

            map.Insert(std::make_pair(a, 100));
            map.Insert(std::make_pair(b, 100));
            map.Insert(std::make_pair(c, 200));
            map.Insert(std::make_pair(d, 200));
            map.Insert(std::make_pair(e, 300));
            map.Insert(std::make_pair(f, 500));
            map.Insert(std::make_pair(g, 900));

            auto it = map.begin();
            auto it2 = map.Find(a);

            Assert::AreEqual(it, it2);

            // Const version
            const HashMap<const Foo, int> constMap(map);
            auto it3 = constMap.cbegin();
            auto it4 = constMap.Find(a);

            Assert::AreEqual(it3, it4);
        }

        TEST_METHOD(TestEnd)
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

            auto it = map.end();
            auto it2 = map.Find(g);
            ++it2;

            Assert::AreEqual(it, it2);

            // Const version
            const HashMap<const Foo, int> constMap(map);
            auto it3 = constMap.cend();
            auto it4 = constMap.Find(g);
            ++it4;

            Assert::AreEqual(it3, it4);
        }

        TEST_METHOD(TestPreIncrement)
        {
            HashMap<const Foo, int>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {++unownedIt; });

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

            auto it = map.begin();
            ++it;
            Assert::AreEqual((*it).first, b);

            ++it; ++it; ++it; ++it;
            Assert::AreEqual((*it).first, f);

            ++it;
            Assert::AreEqual((*it).first, g);

            // Const version
            HashMap<const Foo, int>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {++constUnownedIt; });

            const HashMap<const Foo, int> constMap(map);

            auto constIt = constMap.begin();
            ++constIt;
            Assert::AreEqual((*constIt).first, b);

            ++constIt; ++constIt; ++constIt; ++constIt;
            Assert::AreEqual((*constIt).first, f);

            ++constIt;
            Assert::AreEqual((*constIt).first, g);

            HashMap<const Foo, int>::ConstIterator test;
            test = it;
        }

        TEST_METHOD(TestPostIncrement)
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

            auto it = map.begin();
            it++;
            Assert::AreEqual(it->first, b);

            it++; it++; it++; it++;
            Assert::AreEqual(it->first, f);

            it++;
            Assert::AreEqual((*it).first, g);

            // Const version
            const HashMap<const Foo, int> constMap(map);

            auto constIt = constMap.begin();
            constIt++;
            Assert::AreEqual(constIt->first, b);

            constIt++; constIt++; constIt++; constIt++;
            Assert::AreEqual(constIt->first, f);

            constIt++;
            Assert::AreEqual((*constIt).first, g);
        }

        TEST_METHOD(TestEquality)
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

            auto it = map.begin();
            auto it2 = map.Find(a);
            Assert::IsTrue(it == it2);

            ++it; ++it; ++it; ++it; ++it; ++it;
            auto it3 = map.Find(g);
            Assert::IsTrue(it == it3);

            ++it;
            Assert::IsTrue(it == map.end());
            Assert::IsTrue(it3 != it2);

            // Const version
            const HashMap<const Foo, int> constMap(map);
            auto constIt = constMap.begin();
            auto constIt2 = constMap.Find(a);
            Assert::IsTrue(constIt == constIt2);

            ++constIt; ++constIt; ++constIt; ++constIt; ++constIt; ++constIt;
            auto constIt3 = constMap.Find(g);
            Assert::IsTrue(constIt == constIt3);

            ++constIt;
            Assert::IsTrue(constIt == constMap.end());
            Assert::IsTrue(constIt3 != constIt2);

        }

    private:

        static _CrtMemState sStartMemState;


    };

    _CrtMemState HashMapIteratorTests::sStartMemState;
}
