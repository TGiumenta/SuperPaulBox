#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include <exception>
#include "Vector.h"

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

    template<>
    inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
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
    inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
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

namespace UnitTestLibraryDesktop
{
    struct CoolIncrementStrategy final
    {
        size_t operator()(size_t /*size*/, size_t capacity) const
        {
            return capacity + 100;
        }
    };

	struct PrettyBadIncrementStrategy final
	{
		size_t operator()(size_t /*size*/, size_t capacity) const 
		{
			return capacity - 1;
		}
	};

    struct EvenWorseIncrementStrategy final
    {
        size_t operator()(size_t /*size*/, size_t capacity) const
        {
            return capacity * 0;
        }
    };


	TEST_CLASS(VectorTests)
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
			Vector<Foo> v(size_t(10));
			Assert::IsTrue(v.IsEmpty());
			Assert::AreEqual(v.Capacity(), size_t(10));

			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			Vector<Foo> v2{ a, b, c, d };
			Assert::AreEqual(v2.Size(), size_t(4));
			Assert::AreEqual(v2.Front(), a);
			Assert::AreEqual(v2.Back(), d);

			
		}

		TEST_METHOD(TestCopyConstructorAndAssignment)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
			Vector<Foo> v{ a, b, c, d };

			Vector<Foo> v2(v);
			Assert::AreEqual(v[0], v2[0]);
			Assert::AreEqual(v[1], v2[1]);
			Assert::AreEqual(v[2], v2[2]);
			Assert::AreEqual(v[3], v2[3]);
			Assert::AreEqual(v.Size(), v2.Size());
			Assert::AreEqual(v.Capacity(), v2.Capacity());

			Vector<Foo> v3{ d, a, b, c, a };
			v3 = v;
            Assert::AreEqual(v[0], v3[0]);
            Assert::AreEqual(v[1], v3[1]);
            Assert::AreEqual(v[2], v3[2]);
            Assert::AreEqual(v[3], v3[3]);
            Assert::AreEqual(v.Size(), v3.Size());

			// Should this be true??
            //Assert::AreEqual(v.Capacity(), v3.Capacity());

		}

		TEST_METHOD(TestMoveSemantics)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

			Vector<Foo> v2(std::move(v));
			Assert::IsTrue(v.IsEmpty());
			Assert::ExpectException<std::exception>([&v] { v[0]; });

            Assert::AreEqual(v2[0], a);
            Assert::AreEqual(v2[1], b);
            Assert::AreEqual(v2[2], c);
            Assert::AreEqual(v2[3], d);

			Vector<Foo> v3{ d, a, b, c, a };

			v3 = std::move(v2);
            Assert::IsTrue(v2.IsEmpty());
            Assert::ExpectException<std::exception>([&v2] { v2[0]; });
            Assert::AreEqual(v3[0], a);
            Assert::AreEqual(v3[1], b);
            Assert::AreEqual(v3[2], c);
            Assert::AreEqual(v3[3], d);
		}
		
		TEST_METHOD(TestPushBack)
		{
			Vector<Foo> v(size_t(3));
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			v.PushBack(a);
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(1));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(b);
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(2));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(c);
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(3));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(d);
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(4));
			Assert::AreEqual(v.Capacity(), size_t(4));

			v.PushBack<PrettyBadIncrementStrategy>(a);
            Assert::AreEqual(v.Size(), size_t(5));
            Assert::AreEqual(v.Capacity(), size_t(5));

			v.PushBack<EvenWorseIncrementStrategy>(a);
            Assert::AreEqual(v.Size(), size_t(6));
            Assert::AreEqual(v.Capacity(), size_t(6));

			v.PushBack<CoolIncrementStrategy>(a);
            Assert::AreEqual(v.Size(), size_t(7));
            Assert::AreEqual(v.Capacity(), size_t(106));
		}

		TEST_METHOD(TestPushBackMoveSemantics)
		{
			Vector<Foo> v(size_t(3));
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Foo d(4);

			v.PushBack(std::move(a));
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(1));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(std::move(b));
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(2));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(std::move(c));
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(3));
			Assert::AreEqual(v.Capacity(), size_t(3));

			v.PushBack(std::move(d));
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(4));
			Assert::AreEqual(v.Capacity(), size_t(4));

            // Custom increment functors
            v.PushBack<PrettyBadIncrementStrategy>(std::move(a));
            Assert::AreEqual(v.Size(), size_t(5));
            Assert::AreEqual(v.Capacity(), size_t(5));

            v.PushBack<EvenWorseIncrementStrategy>(std::move(a));
            Assert::AreEqual(v.Size(), size_t(6));
            Assert::AreEqual(v.Capacity(), size_t(6));

            v.PushBack<CoolIncrementStrategy>(std::move(a));
            Assert::AreEqual(v.Size(), size_t(7));
            Assert::AreEqual(v.Capacity(), size_t(106));
		}

		TEST_METHOD(TestBracketOperator)
		{
            Vector<Foo> v(size_t(3));
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);

            v.PushBack(a);
            v.PushBack(b);
            v.PushBack(c);
            v.PushBack(d);

			Assert::AreEqual(v[0], a);
			Assert::AreEqual(v[1], b);
			Assert::AreEqual(v[2], c);
			Assert::AreEqual(v[3], d);

			Assert::AreNotEqual(v[1], a);

			Assert::ExpectException<std::exception>([&v] { v[4]; });

			// const version
            const Vector<Foo> v2(v);

            Assert::AreEqual(v2[0], a);
            Assert::AreEqual(v2[1], b);
            Assert::AreEqual(v2[2], c);
            Assert::AreEqual(v2[3], d);

            Assert::AreNotEqual(v2[1], a);

            Assert::ExpectException<std::exception>([&v2] { v2[4]; });
		}

		TEST_METHOD(TestAt)
		{
			// non-const version
			Vector<Foo> v(size_t(3));
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			v.PushBack(a);
			v.PushBack(b);
			v.PushBack(c);
			v.PushBack(d);

			Assert::AreEqual(v.At(0), a);
			Assert::AreEqual(v.At(1), b);
			Assert::AreEqual(v.At(2), c);
			Assert::AreEqual(v.At(3), d);
			Assert::AreNotEqual(v.At(1), a);
			Assert::ExpectException<std::exception>([&v] { v.At(4); });

			// const version
			const Vector<Foo> v2{ a, b, c, d };
			Assert::AreEqual(v2.At(0), a);
			Assert::AreEqual(v2.At(1), b);
			Assert::AreEqual(v2.At(2), c);
			Assert::AreEqual(v2.At(3), d);
			Assert::AreNotEqual(v2.At(1), a);
			Assert::ExpectException<std::exception>([&v2] { v2.At(4); });
		}

		TEST_METHOD(TestPopBack)
		{
			Vector<Foo> v(size_t(3));
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			v.PushBack(a);
			v.PushBack(b);
			v.PushBack(c);
			v.PushBack(d);

			v.PopBack();
			Assert::IsFalse(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(3));
			Assert::AreEqual(v.Capacity(), size_t(4));

			v.PopBack();
			Assert::AreEqual(v.Size(), size_t(2));
			Assert::AreEqual(v.Capacity(), size_t(4));

			v.PopBack();
			Assert::AreEqual(v.Size(), size_t(1));
			Assert::AreEqual(v.Capacity(), size_t(4));

			v.PopBack();
			Assert::IsTrue(v.IsEmpty());
			Assert::AreEqual(v.Size(), size_t(0));
			Assert::AreEqual(v.Capacity(), size_t(4));

			v.PopBack();
		}

		TEST_METHOD(TestFrontBack)
		{
			// non-const versions
			Vector<Foo> v(size_t(3));
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			Assert::ExpectException<std::exception>([&v] { v.Front(); });
			Assert::ExpectException<std::exception>([&v] { v.Back(); });

			v.PushBack(a);
			Assert::AreEqual(v.Front(), a);
			Assert::AreEqual(v.Back(), a);

			v.PushBack(b);
			v.PushBack(c);
			v.PushBack(d);
			Assert::AreEqual(v.Front(), a);
			Assert::AreEqual(v.Back(), d);

			// const versions
			const Vector<Foo> v2{ a, b, c, d };
			Assert::AreEqual(v2.Front(), a);
			Assert::AreEqual(v2.Back(), d);

			const Vector<Foo> v3;
			Assert::ExpectException<std::exception>([&v3] { v3.Front(); });
			Assert::ExpectException<std::exception>([&v3] { v3.Back(); });
		}

		TEST_METHOD(TestFind)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
			const Foo e(5);
            Vector<Foo> v{ a, b, c, d };

			auto itA = v.Find(a);
			Assert::AreEqual(*itA, a);

			auto itB = v.Find(b);
            Assert::AreEqual(*itB, b);

			auto itD = v.Find(d);
			Assert::AreEqual(*itD, d);

			auto itE = v.Find(e);
			Assert::AreEqual(itE, v.end());

			// const version
			const Vector<Foo> v2(v);
            auto itA2 = v2.Find(a);
            Assert::AreEqual(*itA2, a);

            auto itB2 = v2.Find(b);
            Assert::AreEqual(*itB2, b);

            auto itD2 = v2.Find(d);
            Assert::AreEqual(*itD2, d);

            auto itE2 = v2.Find(e);
            Assert::AreEqual(itE2, v2.end());
		}

		TEST_METHOD(TestRemove)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            const Foo e(5);
            Vector<Foo> v{ a, b, c, d };

			size_t test = sizeof(Foo);
			test;

			v.Remove(b);
			Assert::AreEqual(v[0], a);
			Assert::AreEqual(v[1], c);
			Assert::AreEqual(v[2], d);
			Assert::ExpectException<std::exception>([&v] { v[3]; });

			Assert::IsFalse(v.Remove(e));

		}

		TEST_METHOD(TestRangedRemove)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            const Foo e(5);
            Vector<Foo> v{ a, b, c, d, a, b, c, d, a, b, c };

			auto startIt = v.begin();
			startIt++;
			startIt++;
			startIt++;

			auto endIt = startIt;
			endIt++;
			endIt++;
			endIt++;

			// { a, b, c, || d, a, b, || c, d, a, b, c };
			v.Remove(startIt, endIt);
			// { a, b, c, c, d, a, b, c };

			Assert::AreEqual(v.Size(), size_t(8));
			Assert::AreEqual(v[0], a);
			Assert::AreEqual(v[1], b);
			Assert::AreEqual(v[2], c);
			Assert::AreEqual(v[3], c);
			Assert::AreEqual(v[4], d);
			Assert::AreEqual(v[5], a);
			Assert::AreEqual(v[6], b);
			Assert::AreEqual(v[7], c);
			Assert::ExpectException<std::exception>([&v] { v[8]; });

			// startIt after endIt
			Assert::ExpectException<std::exception>([&v, &endIt, &startIt] { v.Remove(endIt, startIt); });
			
			Vector<Foo> v2(v);
			auto otherIt = v2.end();
			Assert::IsFalse(v.Remove(startIt, otherIt));

			v.Resize(2);
		}

		TEST_METHOD(TestShrinkToFit)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            const Foo e(5);
            Vector<Foo> v{ a, b, c, d, a, b, c, d, a, b, c };
			v.Reserve(100);
			Assert::AreEqual(v.Capacity(), size_t(100));

			v.ShrinkToFit();
			Assert::AreEqual(v.Capacity(), size_t(11));
		}

	private:

		static _CrtMemState sStartMemState;


	};

	_CrtMemState VectorTests::sStartMemState;
}
