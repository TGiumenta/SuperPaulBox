#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "SList.h"
#include "foo.h"
#include <exception>

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
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
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
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
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
	TEST_CLASS(SListIteratorTests)
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

		TEST_METHOD(TestIterator)
		{
			SList<int> list;
			list.PushBack(1);
			list.PushBack(2);
			list.PushBack(3);

			for (auto it = list.begin(); it != list.end(); ++it)
			{
				*it = 6;
			}

			for (auto it = list.begin(); it != list.end(); ++it)
			{
				Assert::AreEqual(*it, 6);
			}
		}

		TEST_METHOD(TestInsertAfter)
		{
			SList<Foo> fooList;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			fooList.PushBack(a);

			auto it = fooList.begin();
			fooList.InsertAfter(it, b);
			Assert::AreEqual(fooList.Front(), a);
			Assert::AreEqual(fooList.Back(), b);

			fooList.InsertAfter(it, c);
			Assert::AreEqual(fooList.Front(), a);
			Assert::AreEqual(fooList.Back(), b);
			Assert::AreEqual(fooList.Size(), size_t(3));

			fooList.InsertAfter(fooList.end(), d);

			SList<Foo>::Iterator unownedIt;
			Assert::ExpectException<std::exception>([&fooList, &unownedIt, &a] { fooList.InsertAfter(unownedIt, a); });
		}

		TEST_METHOD(TestFind)
		{
			SList<Foo> fooList;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			fooList.PushBack(a);
			fooList.PushBack(b);
			fooList.PushBack(c);

			Assert::AreEqual(fooList.Find(a), fooList.begin());
			Assert::AreEqual(fooList.Find(d), fooList.end());
			Assert::AreNotEqual(fooList.Find(b), fooList.begin());
			Assert::AreNotEqual(fooList.Find(c), fooList.begin());

			const SList<Foo> constFooList(fooList);

			Assert::AreEqual(constFooList.Find(a), constFooList.begin());
			Assert::AreEqual(constFooList.Find(d), constFooList.end());
			Assert::AreNotEqual(constFooList.Find(b), constFooList.begin());
			Assert::AreNotEqual(constFooList.Find(c), constFooList.begin());
		}

		TEST_METHOD(TestRemove)
		{
			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			list.Remove(d);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			list.Remove(a);
			Assert::AreEqual(list.Size(), size_t(2));
			Assert::AreEqual(list.Front(), b);
			Assert::AreEqual(list.Back(), c);

			list.Remove(b);
			Assert::AreEqual(list.Size(), size_t(1));
			Assert::AreEqual(list.Front(), c);
			Assert::AreEqual(list.Back(), c);

			list.Remove(c);

			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), size_t(0));

			SList<Foo>::Iterator unownedIt;
			Assert::ExpectException<std::exception>([&list, &unownedIt] { list.Remove(unownedIt); });
		}

		TEST_METHOD(TestIteratorToConstIterator)
		{
			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			auto itBegin = list.begin();
			SList<Foo>::ConstIterator itBeginConst(itBegin);
			itBeginConst = itBegin;
		}

		TEST_METHOD(TestBeginEnd)
		{
			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			auto itBegin = list.begin();
			auto itEnd = list.end();
			auto itCBegin = list.cbegin();
			auto itCEnd = list.cend();

			const SList<Foo> constList(list);
			auto constItBegin = constList.begin();
			auto constItEnd = constList.end();

			Assert::AreEqual(*itBegin, a);
			Assert::AreEqual(*itCBegin, a);
			Assert::AreEqual(*constItBegin, a);
		}

		TEST_METHOD(TestPrefixIncrement)
		{
			SList<Foo>::Iterator unownedIt;
			Assert::ExpectException<std::exception>([&unownedIt] {++unownedIt; });

			SList<Foo>::ConstIterator unownedConstIt;
			Assert::ExpectException<std::exception>([&unownedConstIt] {++unownedConstIt; });

			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			auto it = list.begin();
			++it;
			++it;
			Assert::AreEqual(*it, c);

			auto constIt = list.cbegin();
			++constIt;
			++constIt;
			Assert::AreEqual(*constIt, c);
		}

		TEST_METHOD(TestPostfixIncrement)
		{
			SList<Foo>::Iterator unownedIt;
			Assert::ExpectException<std::exception>([&unownedIt] {unownedIt++; });

			SList<Foo>::ConstIterator unownedConstIt;
			Assert::ExpectException<std::exception>([&unownedConstIt] {unownedConstIt++; });

			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			auto it = list.begin();
			it++;
			it++;
			Assert::AreEqual(*it, c);

			auto constIt = list.cbegin();
			constIt++;
			constIt++;
			Assert::AreEqual(*constIt, c);
		}

		TEST_METHOD(TestDereferenceIterator)
		{
			SList<Foo> list;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			auto it = list.begin();
			Assert::AreEqual(*it, a);

			auto it2 = list.end();
			Assert::ExpectException<std::exception>([&it2] { *it2; });
		}

		TEST_METHOD(TestIteratorEquality)
		{
			SList<Foo> list;
			SList<Foo> list2;
			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);
			list2.PushBack(a);
			list2.PushBack(b);
			list2.PushBack(c);
			list2.PushBack(d);

			auto it = list.begin();
			auto it2 = list2.begin();
			Assert::IsTrue(it != it2);
			Assert::AreNotEqual(it, it2);

			auto it3 = list.end();
			auto it4 = list.end();
			Assert::IsFalse(it3 != it4);
			Assert::AreEqual(it3, it4);

			auto it5 = list.cbegin();
			auto it6 = list2.cbegin();
			Assert::IsTrue(it5 != it6);
			Assert::AreNotEqual(it5, it6);

			auto it7 = list.cend();
			auto it8 = list.cend();
			Assert::IsFalse(it7 != it8);
			Assert::AreEqual(it7, it8);
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListIteratorTests::sStartMemState;
}