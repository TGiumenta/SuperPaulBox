#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> inline std::wstring ToString<Foo>(const Foo& t) 
	{
		RETURN_WIDE_STRING(t.Data()); 
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTests)
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
		TEST_METHOD(TestContructor)
		{
			Foo a;
			Assert::AreEqual(0, a.Data());

			const int value = 10;
			Foo b(value);
			Assert::AreEqual(value, b.Data());
		}

		TEST_METHOD(TestCopySemantics)
		{
			const int value = 10;
			Foo a(value);
			Assert::AreEqual(value, a.Data());

			Foo b = a;
			Assert::AreEqual(a, b);

			Foo c;
			Assert::IsTrue(a != c);

			c = a;
			Assert::AreEqual(a, c);
		}

		TEST_METHOD(TestMoveSemantics)
		{
			const int value = 10;
			Foo a(value);
			Assert::AreEqual(value, a.Data());

			// This is what std::move does internally
			//Foo&& aRef = static_cast<Foo&&>(a);

			Foo b(std::move(a));

			Assert::ExpectException<std::exception>([&a] {a.Data(); });

			Foo c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(value, c.Data());
			Assert::ExpectException<std::exception>([&b, &c] {b == c;});
		}

	private:

		static _CrtMemState sStartMemState;


	};

	_CrtMemState FooTests::sStartMemState;
}
