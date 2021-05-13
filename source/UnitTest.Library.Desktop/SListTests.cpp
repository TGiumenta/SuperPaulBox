#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "SList.h"
#include "foo.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTests)
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

		TEST_METHOD(TestConstructor)
		{
			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			SList<Foo> fooList;
			SList<Foo> emptyListCopy(fooList);

			fooList.PushBack(Foo(3));
			fooList.PushBack(Foo(4));
			fooList.PushBack(Foo(5));
			fooList.PushFront(Foo(2));
			fooList.PushFront(Foo(1));

			SList<Foo> fooList2(fooList);

			fooList.Clear();

			Assert::AreEqual(fooList2.Size(), size_t(5));
			Assert::AreEqual(fooList2.Front().Data(), 1);
			Assert::AreEqual(fooList2.Back().Data(), 5);
		}

		TEST_METHOD(TestCopyAssignment)
		{
			SList<Foo> fooList;
			SList<Foo> emptyListCopy;
			emptyListCopy = fooList;

			fooList.PushBack(Foo(3));
			fooList.PushBack(Foo(4));
			fooList.PushBack(Foo(5));
			fooList.PushFront(Foo(2));
			fooList.PushFront(Foo(1));

			SList<Foo> fooList2;
			fooList2.PushBack(Foo(8));
			fooList2.PushBack(Foo(9));
			fooList2.PushBack(Foo(10));

			fooList2 = fooList;

			Assert::AreEqual(fooList2.Size(), size_t(5));
			Assert::AreEqual(fooList2.Front().Data(), 1);
			Assert::AreEqual(fooList2.Back().Data(), 5);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			SList<Foo> fooList;
			fooList.PushBack(Foo(3));
			fooList.PushBack(Foo(4));
			fooList.PushBack(Foo(5));
			fooList.PushFront(Foo(2));
			fooList.PushFront(Foo(1));

			SList<Foo> fooList2(std::move(fooList));

			Assert::AreEqual(fooList2.Size(), size_t(5));
			Assert::AreEqual(fooList2.Front().Data(), 1);
			Assert::AreEqual(fooList2.Back().Data(), 5);

			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), size_t(0));
		}

		TEST_METHOD(TestMoveAssignment)
		{
			SList<Foo> fooList;
			fooList.PushBack(Foo(3));
			fooList.PushBack(Foo(4));
			fooList.PushBack(Foo(5));
			fooList.PushFront(Foo(2));
			fooList.PushFront(Foo(1));

			SList<Foo> fooList2;
			fooList2.PushBack(Foo(8));
			fooList2.PushBack(Foo(9));
			fooList2.PushBack(Foo(10));

			fooList2 = std::move(fooList);

			Assert::AreEqual(fooList2.Size(), size_t(5));
			Assert::AreEqual(fooList2.Front().Data(), 1);
			Assert::AreEqual(fooList2.Back().Data(), 5);

			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), size_t(0));
		}

		TEST_METHOD(TestPushFront)
		{
			int value1 = 5;
			int value2 = 10;
			SList<Foo> fooList;

			fooList.PushFront(Foo(value1));
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Front().Data(), value1);
			Assert::AreEqual(fooList.Back().Data(), value1);
			Assert::AreEqual(fooList.Front().Data(), fooList.Back().Data());

			fooList.PushFront(Foo(value2));
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Front().Data(), value2);
			Assert::AreEqual(fooList.Back().Data(), value1);
		}

		TEST_METHOD(TestPushBack)
		{
			int value1 = 5;
			int value2 = 10;
			SList<Foo> fooList;

			fooList.PushBack(Foo(value1));
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Front().Data(), value1);
			Assert::AreEqual(fooList.Back().Data(), value1);
			Assert::AreEqual(fooList.Front().Data(), fooList.Back().Data());

			fooList.PushBack(Foo(value2));
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Front().Data(), value1);
			Assert::AreEqual(fooList.Back().Data(), value2);
		}
		
		TEST_METHOD(TestPopFront)
		{
			SList<Foo> fooList;
			fooList.PushBack(Foo(1));
			fooList.PushBack(Foo(2));
			fooList.PushBack(Foo(3));

			fooList.PopFront();
			Assert::AreEqual(fooList.Front().Data(), 2);
			Assert::AreEqual(fooList.Back().Data(), 3);

			fooList.PopFront();
			fooList.PopFront();
			Assert::IsTrue(fooList.IsEmpty());

			fooList.PopFront();
		}

		TEST_METHOD(TestPopBack)
		{
			SList<Foo> fooList;
			fooList.PushBack(Foo(1));
			fooList.PushBack(Foo(2));
			fooList.PushBack(Foo(3));

			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), size_t(2));
			Assert::AreEqual(fooList.Back().Data(), 2);
			Assert::AreEqual(fooList.Front().Data(), 1);

			fooList.PopBack();
			Assert::AreEqual(fooList.Back().Data(), fooList.Front().Data());

			fooList.PopBack();
			Assert::IsTrue(fooList.IsEmpty());

			fooList.PopBack();
		}

		TEST_METHOD(TestFront)
		{
			SList<Foo> fooList;
			Assert::ExpectException<std::exception>([&fooList] {fooList.Front(); });

			const SList<Foo> emptyConstList;
			Assert::ExpectException<std::exception>([&emptyConstList] {emptyConstList.Front(); });

			fooList.PushBack(Foo(1));
			fooList.PushBack(Foo(2));
			fooList.PushBack(Foo(3));

			Foo& frontFoo = fooList.Front();
			frontFoo.SetData(5);
			Assert::AreEqual(fooList.Front().Data(), 5);

			// Testing with const lists
			const SList<Foo> constFooList(fooList);
			const Foo& constFrontFoo = constFooList.Front();
			Assert::AreEqual(constFrontFoo.Data(), 5);
		}

		TEST_METHOD(TestBack)
		{
			SList<Foo> fooList;

			Assert::ExpectException<std::exception>([&fooList] {fooList.Back(); });

			const SList<Foo> emptyConstList;
			Assert::ExpectException<std::exception>([&emptyConstList] {emptyConstList.Back(); });


			fooList.PushBack(Foo(1));
			fooList.PushBack(Foo(2));
			fooList.PushBack(Foo(3));

			Foo& backFoo = fooList.Back();
			backFoo.SetData(5);
			Assert::AreEqual(fooList.Back().Data(), 5);

			// Testing with const lists
			const SList<Foo> constFooList(fooList);
			const Foo& constBackFoo = constFooList.Back();
			Assert::AreEqual(constBackFoo.Data(), 5);
		}

		TEST_METHOD(TestSize)
		{
			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), size_t(0));

			fooList.PushBack(Foo(1));
			Assert::AreEqual(fooList.Size(), size_t(1));

			fooList.PushBack(Foo(2));
			Assert::AreEqual(fooList.Size(), size_t(2));

			fooList.PushBack(Foo(3));
			Assert::AreEqual(fooList.Size(), size_t(3));

			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), size_t(2));

			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), size_t(1));

			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), size_t(0));
		}

		TEST_METHOD(TestIsEmpty)
		{
			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());

			fooList.PushBack(Foo(1));
			Assert::IsFalse(fooList.IsEmpty());

			fooList.PopBack();
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(TestClear)
		{
			SList<Foo> fooList;
			fooList.PushBack(Foo(3));
			fooList.PushBack(Foo(4));
			fooList.PushBack(Foo(5));
			fooList.PushFront(Foo(2));
			fooList.PushFront(Foo(1));

			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), size_t(0));
		}

	private:

		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTests::sStartMemState;
}