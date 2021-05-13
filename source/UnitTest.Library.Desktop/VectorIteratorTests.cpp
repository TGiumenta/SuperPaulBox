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
	TEST_CLASS(VectorIteratorTests)
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

		TEST_METHOD(TestPrefixIncrement)
		{
			Vector<Foo>::Iterator unownedIt;
			Assert::ExpectException<std::exception>([&unownedIt] {++unownedIt; });

			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			Vector<Foo> v{ a, b, c, d };

			auto it = v.begin();
			++it;
			++it;
			Assert::AreEqual(*it, c);

			// constIterator versions
			Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {++constUnownedIt; });

			const Vector<Foo> v2{ a, b, c, d };
			auto constIt = v2.begin();
			++constIt;
			++constIt;
			Assert::AreEqual(*constIt, c);
		}

		TEST_METHOD(TestPostfixIncrement)
		{
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt++; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.begin();
            it++;
            it++;
            Assert::AreEqual(*it, c);

			// constIterator versions
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt++; });

            const Vector<Foo> v2{ a, b, c, d };
            auto constIt = v2.begin();
            constIt++;
            constIt++;
            Assert::AreEqual(*constIt, c);

            auto endIt = v2.cend();
            constIt++;
			constIt++;
			constIt++;
			constIt++;
			constIt++;
			Assert::AreEqual(constIt, endIt);
		}

		TEST_METHOD(TestPrefixDecrement)
		{
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {--unownedIt; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.end();
            --it;
            --it;
            Assert::AreEqual(*it, c);

            // constIterator versions
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {--constUnownedIt; });

            const Vector<Foo> v2{ a, b, c, d };
            auto constIt = v2.end();
            --constIt;
            --constIt;
            Assert::AreEqual(*constIt, c);
		}

		TEST_METHOD(TestPostfixDecrement)
		{
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt--; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.end();
            it--;
            it--;
            Assert::AreEqual(*it, c);

            // constIterator versions
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt--; });

            const Vector<Foo> v2{ a, b, c, d };
            auto constIt = v2.end();
            constIt--;
            constIt--;
            Assert::AreEqual(*constIt, c);
		}

		TEST_METHOD(TestDereference)
		{
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {*unownedIt; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };
            auto it = v.begin();
            Assert::AreEqual(*it, a);

			auto endIt = v.end();
			Assert::ExpectException<std::exception>([&endIt] {*endIt; });

			// test constIterator versions
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {*constUnownedIt; });

            const Vector<Foo> v2{ a, b, c, d };
            auto constIt = v2.begin();
            Assert::AreEqual(*constIt, a);

            auto constEndIt = v2.end();
            Assert::ExpectException<std::exception>([&constEndIt] {*constEndIt; });

		}
		TEST_METHOD(TestIteratorEquality)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };
			Vector<Foo> v2{ a, b, c, d };

			auto it = v.begin();
			auto it2 = v2.begin();
			Assert::IsTrue(it != it2);

			auto it3 = v.begin();
			it++;
			it3++;
			Assert::AreEqual(it, it3);

			// const versions
			auto it4 = v.cbegin();
			auto it5 = v2.cbegin();
			Assert::IsTrue(it4 != it5);

			auto it6 = v.cbegin();
			it4++;
			it6++;
			Assert::AreEqual(it4, it6);
		}

		TEST_METHOD(TestConstIteratorFromIterator)
		{
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

			auto it = v.begin();
			++it;
			++it;

            Vector<Foo>::ConstIterator constIt(it);
            Vector<Foo>::ConstIterator constIt2;
            constIt2 = it;

			Assert::AreEqual(*constIt, c);
			Assert::AreEqual(*constIt2, c);
		}

        TEST_METHOD(TestAdditionOperator)
        {
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt + 3; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.begin();

            it = it + 2;
            Assert::AreEqual(*it, c);

            auto it2 = v.begin();
            it2 = it2 + 1000;
            Assert::AreEqual(it2, v.end());

            // Const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt + 3; });

            const Vector<Foo> constV{ a, b, c, d };

            auto constIt = constV.begin();

            constIt = constIt + 2;
            Assert::AreEqual(*constIt, c);

            auto constIt2 = constV.begin();
            constIt2 = constIt2 + 1000;
            Assert::AreEqual(constIt2, constV.end());
        }

        TEST_METHOD(TestSubtractionOperator)
        {
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt - 3; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.end();

            it = it - 2;
            Assert::AreEqual(*it, c);

            auto it2 = v.end();
            it2 = it2 - 1000;
            Assert::AreEqual(it2, v.begin());

            // Const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt - 3; });

            const Vector<Foo> constV{ a, b, c, d };

            auto constIt = constV.end();

            constIt = constIt - 2;
            Assert::AreEqual(*constIt, c);

            auto constIt2 = constV.end();
            constIt2 = constIt2 - 1000;
            Assert::AreEqual(constIt2, constV.begin());
        }

        TEST_METHOD(TestAdditionAssignment)
        {
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt += 3; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.begin();

            it += 2;
            Assert::AreEqual(*it, c);

            auto it2 = v.begin();
            it2 += 1000;
            Assert::AreEqual(it2, v.end());

            // Const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt += 3; });

            const Vector<Foo> constV{ a, b, c, d };

            auto constIt = constV.begin();

            constIt += 2;
            Assert::AreEqual(*constIt, c);

            auto constIt2 = constV.begin();
            constIt2 += 1000;
            Assert::AreEqual(constIt2, constV.end());
        }

        TEST_METHOD(TestSubtractionAssignment)
        {
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt -= 3; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d };

            auto it = v.end();

            it -= 2;
            Assert::AreEqual(*it, c);

            auto it2 = v.end();
            it2 -= 1000;
            Assert::AreEqual(it2, v.begin());

            // Const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt -= 3; });

            const Vector<Foo> constV{ a, b, c, d };

            auto constIt = constV.end();

            constIt -= 2;
            Assert::AreEqual(*constIt, c);

            auto constIt2 = constV.end();
            constIt2 -= 1000;
            Assert::AreEqual(constIt2, constV.begin());
        }

        TEST_METHOD(TestBracketOperator)
        {
            Vector<Foo>::Iterator unownedIt;
            Assert::ExpectException<std::exception>([&unownedIt] {unownedIt[0]; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d, a, b, c, d };

            auto it = v.begin();
            Assert::AreEqual(it[5], b);
            Assert::AreEqual(it[0], a);
            Assert::AreEqual(it[7], d);
            // const version

            Vector<Foo>::ConstIterator constUnownedIt;
            Assert::ExpectException<std::exception>([&constUnownedIt] {constUnownedIt[0]; });

            const Vector<Foo> constV(v);

            auto constIt = constV.begin();
            Assert::AreEqual(constIt[5], b);
            Assert::AreEqual(constIt[0], a);
            Assert::AreEqual(constIt[7], d);
        }

        TEST_METHOD(TestLessThan)
        {
            Vector<Foo>::Iterator unownedIt;
            Vector<Foo>::Iterator unownedIt2;
            Assert::ExpectException<std::exception>([&unownedIt, &unownedIt2] {unownedIt < unownedIt2; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d, a, b, c, d };
            Vector<Foo> v2(v);

            auto it = v.begin();
            auto it2 = v.begin();
            Assert::IsFalse(it < it2);
            ++it2;
            Assert::IsTrue(it < it2);

            auto otherIt = v2.begin();
            Assert::ExpectException<std::exception>([&it, &otherIt] {it < otherIt; });

            // const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Vector<Foo>::ConstIterator constUnownedIt2;
            Assert::ExpectException<std::exception>([&constUnownedIt, &constUnownedIt2] {constUnownedIt < constUnownedIt2; });

            const Vector<Foo> constV{ a, b, c, d, a, b, c, d };
            const Vector<Foo> constV2(v);

            auto constIt = constV.begin();
            auto constIt2 = constV.begin();
            Assert::IsFalse(constIt < constIt2);
            ++constIt2;
            Assert::IsTrue(constIt < constIt2);

            auto constOtherIt = constV2.begin();
            Assert::ExpectException<std::exception>([&constIt, &constOtherIt] {constIt < constOtherIt; });
        }

        TEST_METHOD(TestGreaterThan)
        {
            Vector<Foo>::Iterator unownedIt;
            Vector<Foo>::Iterator unownedIt2;
            Assert::ExpectException<std::exception>([&unownedIt, &unownedIt2] {unownedIt > unownedIt2; });

            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d, a, b, c, d };
            Vector<Foo> v2(v);

            auto it = v.begin();
            auto it2 = v.begin();
            Assert::IsFalse(it > it2);
            ++it;
            Assert::IsTrue(it > it2);

            auto otherIt = v2.begin();
            Assert::ExpectException<std::exception>([&it, &otherIt] {it > otherIt; });

            // const version
            Vector<Foo>::ConstIterator constUnownedIt;
            Vector<Foo>::ConstIterator constUnownedIt2;
            Assert::ExpectException<std::exception>([&constUnownedIt, &constUnownedIt2] {constUnownedIt > constUnownedIt2; });

            const Vector<Foo> constV{ a, b, c, d, a, b, c, d };
            const Vector<Foo> constV2(v);

            auto constIt = constV.begin();
            auto constIt2 = constV.begin();
            Assert::IsFalse(constIt > constIt2);
            ++constIt;
            Assert::IsTrue(constIt > constIt2);

            auto constOtherIt = constV2.begin();
            Assert::ExpectException<std::exception>([&constIt, &constOtherIt] {constIt > constOtherIt; });
        }

        TEST_METHOD(TestArrow)
        {
            const Foo a(1);
            const Foo b(2);
            const Foo c(3);
            const Foo d(4);
            Vector<Foo> v{ a, b, c, d, a, b, c, d };

            auto it = v.begin();
            it->SetData(8);

            Assert::AreEqual(it->Data(), 8);

            // const version
            const Vector<Foo> constV(v);
            auto constIt = constV.begin();
            Assert::AreEqual(constIt->Data(), 8);

        }

	private:

		static _CrtMemState sStartMemState;


	};

	_CrtMemState VectorIteratorTests::sStartMemState;
}
