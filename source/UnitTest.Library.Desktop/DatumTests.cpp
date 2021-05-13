#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include "Datum.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<> inline std::wstring ToString<Foo>(const Foo& t)
    {
        RETURN_WIDE_STRING(t.Data());
    }

    template<> inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
    {
        std::wstringstream s;
        s << L"("s << ToString(t.x) << L", "s << ToString(t.y) << L", "s << ToString(t.z) << L", "s << ToString(t.w) << L")"s;
        return s.str();
    }
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(DatumTests)
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
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.Reserve(10);

            Datum dString = Datum(Datum::DatumType::String);
            dString.Reserve(10);

            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.Reserve(10);

            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.Reserve(10);

            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.Reserve(10);

            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            dRTTI.Reserve(10);
        }

        TEST_METHOD(TestCopy)
        {
            Datum d = Datum(Datum::DatumType::Integer);
            d.PushBack(1);
            d.PushBack(2);
            d.PushBack(3);
            d.PushBack(4);
            d.PushBack(5);

            Datum d2(d);
            d.Set(300);
            Assert::AreEqual(d2.Get<int>(), 1);

            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");

            Datum d3(dString);
            Assert::AreEqual(d3.Get<std::string>(), "one"s);

            Datum d4 = Datum(Datum::DatumType::Matrix);
            d = dString;
            dString = d2;

            Datum externaldInt(Datum::DatumType::Integer);
            int intArray[] = { 1, 2, 3, 4, 5 };
            externaldInt.SetStorage(intArray, 5);

            d = externaldInt;
            d = dString;

            Datum anotherOne(externaldInt);
            d = std::move(anotherOne);
        }

        TEST_METHOD(TestMoveSemantics)
        {
            Datum d = Datum(Datum::DatumType::Integer);
            d.PushBack(1);
            d.PushBack(2);
            d.PushBack(3);
            d.PushBack(4);
            d.PushBack(5);

            Datum d2(std::move(d));

            Datum d3 = Datum(Datum::DatumType::Integer);
            d3.PushBack(1);
            d3.PushBack(2);
            d3.PushBack(3);
            d3.PushBack(4);
            d3.PushBack(5);
            d3.PushBack(5);
            d3.PushBack(5);

            d3 = std::move(d2);
            Assert::AreEqual(d3.Size(), size_t(5));
            Assert::AreEqual(d3.Capacity(), size_t(5));
        }

        TEST_METHOD(TestAssignmentOverload)
        {
            Datum dInt;
            dInt = 5;
            Assert::AreEqual(dInt.Get<int>(), 5);
            dInt = 6;

            Datum dString;
            dString = "test"s;
            Assert::AreEqual(dString.Get<std::string>(), "test"s);
            dString = "yee"s;

            Datum dFloat;
            dFloat = 5.0f;
            Assert::AreEqual(dFloat.Get<float>(), 5.0f);
            dFloat = 7.0f;

            Datum dVector;
            dVector = glm::vec4(1, 1, 1, 1);
            Assert::IsTrue(dVector.Get<glm::vec4>() == glm::vec4(1, 1, 1, 1));
            dVector = glm::vec4(2, 1, 1, 1);

            Datum dMatrix;
            dMatrix = glm::mat4(1.0f);
            Assert::IsTrue(dMatrix.Get<glm::mat4>() == glm::mat4(1.0f));
            dMatrix = glm::mat4(3.0f);

            Datum dRTTI;
            Foo a(1), b(2);
            dRTTI = &a;
            Assert::IsTrue(dRTTI.Get<RTTI*>() == &a);
            dRTTI = &b;

            Datum dString2 = Datum(Datum::DatumType::String);
            Assert::ExpectException<std::exception>([&dString2] { dString2 = 1; });
            Assert::ExpectException<std::exception>([&dString2] { dString2 = 1.0f; });
            Assert::ExpectException<std::exception>([&dString2] { dString2 = glm::vec4(1, 1, 1, 1); });
            Assert::ExpectException<std::exception>([&dString2] { dString2 = glm::mat4(1.0f); });
            Assert::ExpectException<std::exception>([&dString2, &a] { dString2 = &a; });

            Datum dInt2 = Datum(Datum::DatumType::Integer);
            Assert::ExpectException<std::exception>([&dInt2] { dInt2 = "yeee"s; });
        }

        TEST_METHOD(TestEquality)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.Reserve(3);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);

            Datum dInt2(dInt);
            Assert::IsTrue(dInt == dInt2);

            dInt.Set(50);
            Assert::IsTrue(dInt != dInt2);

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");

            Datum dString2(dString);
            Assert::IsTrue(dString == dString2);

            dString.Set("bleh"s);
            Assert::IsFalse(dString == dString2);

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);

            Datum dFloat2(dFloat);
            Assert::IsTrue(dFloat == dFloat2);

            dFloat.Set(50.0f);
            Assert::IsFalse(dFloat == dFloat2);

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));

            Datum dVector2(dVector);
            Assert::IsTrue(dVector == dVector2);

            dVector.Set(glm::vec4(15, 15, 1, 1));
            Assert::IsFalse(dVector == dVector2);

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));

            Datum dMatrix2(dMatrix);
            Assert::IsTrue(dMatrix == dMatrix2);

            dMatrix.Set(glm::mat4(50));
            Assert::IsFalse(dMatrix == dMatrix2);

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);

            Datum dRTTI2(dRTTI);
            Assert::IsTrue(dRTTI == dRTTI2);

            dRTTI.Set(&f);
            Assert::IsFalse(dRTTI == dRTTI2);

            // Scalar equality
            Assert::IsTrue(dInt != 1);
            Assert::IsTrue(dString != "abcd"s);
            Assert::IsTrue(dFloat != 1.0f);
            Assert::IsTrue(dVector != glm::vec4(1, 2, 3, 4));
            Assert::IsTrue(dMatrix != glm::mat4(1));
            Assert::IsTrue(dRTTI != &c);

            Datum unknown;
            Datum unknown2;
            Assert::IsTrue(unknown == unknown2);
        }

        TEST_METHOD(TestGet)
        {
            Datum d = Datum(Datum::DatumType::Integer);
            d.PushBack(1);
            d.PushBack(2);
            d.PushBack(3);
            d.PushBack(4);
            d.PushBack(5);

            int& data = d.Get<int>(2);
            data = 42;
            Assert::AreEqual(d.Get<int>(2), 42);

            Assert::ExpectException<std::exception>([&d] { d.Get<int>(100); });
            Assert::ExpectException<std::exception>([&d] { d.Get<float>(); });
        }

        TEST_METHOD(TestSet)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            Assert::ExpectException<std::exception>([&dInt] { dInt.Set(100); });
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);

            dInt.Set(400);
            Assert::AreEqual(dInt.Get<int>(), 400);

            dInt.Set(130, 2);
            Assert::AreEqual(dInt.Get<int>(2), 130);

            Assert::ExpectException<std::exception>([&dInt] { dInt.Set(100, 100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Set("hi"s); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Set(100.0f); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Set(glm::vec4(1, 1, 1, 1)); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Set(glm::mat4(5.0f)); });

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            Assert::ExpectException<std::exception>([&dString] { dString.Set("yeehaw"s); });
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");

            dString.Set("bleh"s);
            Assert::AreEqual(dString.Get<std::string>(), "bleh"s);

            dString.Set("potato"s, 2);
            Assert::AreEqual(dString.Get<std::string>(2), "potato"s);

            Assert::ExpectException<std::exception>([&dString] { dString.Set("yeehaw"s, 100); });
            Assert::ExpectException<std::exception>([&dString] { dString.Set(1); });

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            Assert::ExpectException<std::exception>([&dFloat] { dFloat.Set(100.0f); });
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);

            dFloat.Set(400.0f);
            Assert::AreEqual(dFloat.Get<float>(), 400.0f);

            dFloat.Set(130.0f, 2);
            Assert::AreEqual(dFloat.Get<float>(2), 130.0f);

            Assert::ExpectException<std::exception>([&dFloat] { dFloat.Set(100.0f, 100); });

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            Assert::ExpectException<std::exception>([&dVector] { dVector.Set(glm::vec4(100, 100, 100, 100)); });
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));

            dVector.Set(glm::vec4(100, 100, 100, 100));
            Assert::AreEqual(dVector.Get<glm::vec4>(), glm::vec4(100, 100, 100, 100));

            dVector.Set(glm::vec4(130, 130, 130, 130), 2);
            Assert::AreEqual(dVector.Get<glm::vec4>(2), glm::vec4(130, 130, 130, 130));

            Assert::ExpectException<std::exception>([&dVector] { dVector.Set(glm::vec4(100, 100, 100, 100), 100); });

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            Assert::ExpectException<std::exception>([&dMatrix] { dMatrix.Set(glm::mat4(100.0f)); });
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));

            dMatrix.Set(glm::mat4(100.0f));
            Assert::IsTrue(dMatrix.Get<glm::mat4>() == glm::mat4(100.0f));

            dMatrix.Set(glm::mat4(130.0f), 2);
            Assert::IsTrue(dMatrix.Get<glm::mat4>(2) == glm::mat4(130.0f));

            Assert::ExpectException<std::exception>([&dMatrix] { dMatrix.Set(glm::mat4(100.0f), 100); });

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            
            Assert::ExpectException<std::exception>([&dRTTI, &a] { dRTTI.Set(&a); });
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);

            dRTTI.Set(&f);
            Assert::IsTrue(dRTTI.Get<RTTI*>() == &f);

            dRTTI.Set(&f, 2);
            Assert::IsTrue(dRTTI.Get<RTTI*>(2) == &f);

            Assert::ExpectException<std::exception>([&dRTTI, &f] { dRTTI.Set(&f, 100); });
            Assert::ExpectException<std::exception>([&dInt, &a] { dInt.Set(&a); });

        }

        TEST_METHOD(TestPushBack)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            Assert::AreEqual(dInt.Size(), size_t(0));
            dInt.Reserve(3);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);
            Assert::AreEqual(dInt.Size(), size_t(5));

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            Assert::AreEqual(dString.Size(), size_t(0));
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");
            Assert::AreEqual(dString.Size(), size_t(5));

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            Assert::AreEqual(dFloat.Size(), size_t(0));
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);
            Assert::AreEqual(dFloat.Size(), size_t(5));

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            Assert::AreEqual(dVector.Size(), size_t(0));
            dVector.PushBack(glm::vec4(1,1,1,1));
            dVector.PushBack(glm::vec4(2,2,2,2));
            dVector.PushBack(glm::vec4(3,3,3,3));
            dVector.PushBack(glm::vec4(4,4,4,4));
            dVector.PushBack(glm::vec4(5,5,5,5));
            Assert::AreEqual(dVector.Size(), size_t(5));

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            Assert::AreEqual(dMatrix.Size(), size_t(0));
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));
            Assert::AreEqual(dMatrix.Size(), size_t(5));

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5);
            Assert::AreEqual(dRTTI.Size(), size_t(0));
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);
            Assert::AreEqual(dRTTI.Size(), size_t(5));

            Assert::ExpectException<std::exception>([&dRTTI] { dRTTI.PushBack(100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.PushBack("hi"s); });
            Assert::ExpectException<std::exception>([&dInt] { std::string a = "a";  dInt.PushBack(a); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.PushBack(100.0f); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.PushBack(glm::vec4(1, 1, 1, 1)); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.PushBack(glm::mat4(5.0f)); });
            Assert::ExpectException<std::exception>([&dInt, &a] { dInt.PushBack(&a); });
        }
        
        TEST_METHOD(TestFrontBack)
        {

            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);

            dInt.Front<int>() = 25;
            Assert::AreEqual(dInt.Get<int>(0), 25);

            dInt.Back<int>() = 45;
            Assert::AreEqual(dInt.Get<int>(4), 45);

            const Datum constDInt(dInt);
            Assert::AreEqual(constDInt.Front<int>(), 25);
            Assert::AreEqual(constDInt.Back<int>(), 45);

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");

            dString.Front<std::string>() = "yeehaw"s;
            Assert::AreEqual(dString.Get<std::string>(0), "yeehaw"s);

            dString.Back<std::string>() = "ya"s;
            Assert::AreEqual(dString.Get<std::string>(4), "ya"s);

            const Datum constDString(dString);
            Assert::AreEqual(constDString.Front<std::string>(), "yeehaw"s);
            Assert::AreEqual(constDString.Back<std::string>(), "ya"s);

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);

            dFloat.Front<float>() = 100.0f;
            Assert::AreEqual(dFloat.Get<float>(0), 100.0f);

            dFloat.Back<float>() = 70.0f;
            Assert::AreEqual(dFloat.Get<float>(4), 70.0f);

            const Datum constDFloat(dFloat);
            Assert::AreEqual(constDFloat.Front<float>(), 100.0f);
            Assert::AreEqual(constDFloat.Back<float>(), 70.0f);

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));

            dVector.Front<glm::vec4>() = glm::vec4(100, 100, 100, 100);
            Assert::AreEqual(dVector.Get<glm::vec4>(0), glm::vec4(100, 100, 100, 100));

            dVector.Back<glm::vec4>() = glm::vec4(300, 300, 300, 300);
            Assert::AreEqual(dVector.Get<glm::vec4>(4), glm::vec4(300, 300, 300, 300));

            const Datum constDVector(dVector);
            Assert::AreEqual(constDVector.Front<glm::vec4>(), glm::vec4(100, 100, 100, 100));
            Assert::AreEqual(constDVector.Back<glm::vec4>(), glm::vec4(300, 300, 300, 300));

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));

            dMatrix.Front<glm::mat4>() = glm::mat4(10.0f);
            Assert::IsTrue(dMatrix.Get<glm::mat4>(0) == glm::mat4(10.0f));

            dMatrix.Back<glm::mat4>() = glm::mat4(70.0f);
            Assert::IsTrue(dMatrix.Get<glm::mat4>(4) == glm::mat4(70.0f));

            const Datum constDMatrix(dMatrix);
            Assert::IsTrue(constDMatrix.Front<glm::mat4>() == glm::mat4(10.0f));
            Assert::IsTrue(constDMatrix.Back<glm::mat4>() == glm::mat4(70.0f));

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);

            dRTTI.Front<RTTI*>() = &f;
            Assert::IsTrue(dRTTI.Get<RTTI*>(0) == &f);

            dRTTI.Back<RTTI*>() = &f;
            Assert::IsTrue(dRTTI.Get<RTTI*>(4) == &f);

            const Datum constDRTTI(dRTTI);
            Assert::IsTrue(constDRTTI.Front<RTTI*>() == &f);
            Assert::IsTrue(constDRTTI.Back<RTTI*>() == &f);

            // Exceptions
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<std::string>(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<float>(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<glm::vec4>(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<glm::mat4>(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<RTTI*>(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Front<Scope>(); });
            Assert::ExpectException<std::exception>([&dString] { dString.Front<int>(); });

            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<std::string>(); });
            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<float>(); });
            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<glm::vec4>(); });
            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<glm::mat4>(); });
            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<RTTI*>(); });
            Assert::ExpectException<std::exception>([&constDInt] { constDInt.Front<Scope>(); });
            Assert::ExpectException<std::exception>([&constDString] { constDString.Front<int>(); });

            Datum dIntEmpty(Datum::DatumType::Integer);
            Datum dStringEmpty(Datum::DatumType::String);
            Datum dFloatEmpty(Datum::DatumType::Float);
            Datum dVectorEmpty(Datum::DatumType::Vector);
            Datum dMatrixEmpty(Datum::DatumType::Matrix);
            Datum dRTTIEmpty(Datum::DatumType::RTTI);
            Assert::ExpectException<std::exception>([&dIntEmpty] { dIntEmpty.Front<int>(); });
            Assert::ExpectException<std::exception>([&dStringEmpty] { dStringEmpty.Front<std::string>(); });
            Assert::ExpectException<std::exception>([&dFloatEmpty] { dFloatEmpty.Front<float>(); });
            Assert::ExpectException<std::exception>([&dVectorEmpty] { dVectorEmpty.Front<glm::vec4>(); });
            Assert::ExpectException<std::exception>([&dMatrixEmpty] { dMatrixEmpty.Front<glm::mat4>(); });
            Assert::ExpectException<std::exception>([&dRTTIEmpty] { dRTTIEmpty.Front<RTTI*>(); });

            const Datum constdIntEmpty(Datum::DatumType::Integer);
            const Datum constdStringEmpty(Datum::DatumType::String);
            const Datum constdFloatEmpty(Datum::DatumType::Float);
            const Datum constdVectorEmpty(Datum::DatumType::Vector);
            const Datum constdMatrixEmpty(Datum::DatumType::Matrix);
            const Datum constdRTTIEmpty(Datum::DatumType::RTTI);
            Assert::ExpectException<std::exception>([&constdIntEmpty] { constdIntEmpty.Front<int>(); });
            Assert::ExpectException<std::exception>([&constdStringEmpty] { constdStringEmpty.Front<std::string>(); });
            Assert::ExpectException<std::exception>([&constdFloatEmpty] { constdFloatEmpty.Front<float>(); });
            Assert::ExpectException<std::exception>([&constdVectorEmpty] { constdVectorEmpty.Front<glm::vec4>(); });
            Assert::ExpectException<std::exception>([&constdMatrixEmpty] { constdMatrixEmpty.Front<glm::mat4>(); });
            Assert::ExpectException<std::exception>([&constdRTTIEmpty] { constdRTTIEmpty.Front<RTTI*>(); });
        }

        TEST_METHOD(TestFind)
        {
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);

            Assert::AreEqual(dInt.Find(3), size_t(2));
            Assert::AreEqual(dInt.Find(5), size_t(4));
            Assert::AreEqual(dInt.Find(123145), size_t(5));

            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5);
            Assert::AreEqual(dRTTI.Size(), size_t(0));
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);
            Assert::AreEqual(dRTTI.Size(), size_t(5));

            Assert::ExpectException<std::exception>([&dRTTI] { dRTTI.Find(100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Find("hi"s); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Find(100.0f); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Find(glm::vec4(1, 1, 1, 1)); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Find(glm::mat4(5.0f)); });
            Assert::ExpectException<std::exception>([&dInt, &a] { dInt.Find(&a); });

        }

        TEST_METHOD(TestRemove)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            Assert::ExpectException<std::exception>([&dInt] { dInt.RemoveAt(100); });
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);

            dInt.RemoveAt(2);
            Assert::AreEqual(dInt.Get<int>(2), 4);
            Assert::AreEqual(dInt.Get<int>(3), 5);

            dInt.Remove(1);
            Assert::AreEqual(dInt.Get<int>(0), 2);
            Assert::AreEqual(dInt.Get<int>(1), 4);
            Assert::AreEqual(dInt.Get<int>(2), 5);

            Assert::IsFalse(dInt.Remove(7));


            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");

            dString.RemoveAt(2);
            Assert::AreEqual(dString.Get<std::string>(2), "four"s);
            Assert::AreEqual(dString.Get<std::string>(3), "five"s);

            dString.Remove("one"s);
            Assert::AreEqual(dString.Get<std::string>(0), "two"s);
            Assert::AreEqual(dString.Get<std::string>(1), "four"s);
            Assert::AreEqual(dString.Get<std::string>(2), "five"s);

            Assert::IsFalse(dString.Remove("letters"));

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);

            dFloat.RemoveAt(2);
            Assert::AreEqual(dFloat.Get<float>(2), 4.0f);
            Assert::AreEqual(dFloat.Get<float>(3), 5.0f);

            dFloat.Remove(1.0f);
            Assert::AreEqual(dFloat.Get<float>(0), 2.0f);
            Assert::AreEqual(dFloat.Get<float>(1), 4.0f);
            Assert::AreEqual(dFloat.Get<float>(2), 5.0f);

            Assert::IsFalse(dFloat.Remove(7.0f));

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));

            dVector.RemoveAt(2);
            Assert::AreEqual(dVector.Get<glm::vec4>(2), glm::vec4(4, 4, 4, 4));
            Assert::AreEqual(dVector.Get<glm::vec4>(3), glm::vec4(5, 5, 5, 5));

            dVector.Remove(glm::vec4(1, 1, 1, 1));
            Assert::AreEqual(dVector.Get<glm::vec4>(0), glm::vec4(2, 2, 2, 2));
            Assert::AreEqual(dVector.Get<glm::vec4>(1), glm::vec4(4, 4, 4, 4));
            Assert::AreEqual(dVector.Get<glm::vec4>(2), glm::vec4(5, 5, 5, 5));

            Assert::IsFalse(dVector.Remove(glm::vec4(7567, 5, 5, 5)));

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));

            dMatrix.RemoveAt(2);
            Assert::IsTrue(dMatrix.Get<glm::mat4>(2) == glm::mat4(4.0f));
            Assert::IsTrue(dMatrix.Get<glm::mat4>(3) == glm::mat4(5.0f));

            dMatrix.Remove(glm::mat4(1.0f));
            Assert::IsTrue(dMatrix.Get<glm::mat4>(0) == glm::mat4(2.0f));
            Assert::IsTrue(dMatrix.Get<glm::mat4>(1) == glm::mat4(4.0f));
            Assert::IsTrue(dMatrix.Get<glm::mat4>(2) == glm::mat4(5.0f));

            Assert::IsFalse(dMatrix.Remove(glm::mat4(756.0f)));

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);

            dRTTI.RemoveAt(2);
            Assert::IsTrue(dRTTI.Get<RTTI*>(2) == &d);
            Assert::IsTrue(dRTTI.Get<RTTI*>(3) == &e);

            dRTTI.Remove(&a);
            Assert::IsTrue(dRTTI.Get<RTTI*>(0) == &b);
            Assert::IsTrue(dRTTI.Get<RTTI*>(1) == &d);
            Assert::IsTrue(dRTTI.Get<RTTI*>(2) == &e);

            Assert::IsFalse(dRTTI.Remove(&f));
        }

        TEST_METHOD(TestPopBack)
        {
            Datum d = Datum(Datum::DatumType::Integer);
            d.PushBack(1);
            d.PushBack(2);
            d.PushBack(3);
            d.PushBack(4);
            d.PushBack(5);

            d.PopBack();
            Assert::AreEqual(d.Size(), size_t(4));
            Assert::AreEqual(d.Capacity(), size_t(5));

            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");
            dString.PopBack();
            dString.PopBack();
        }

        TEST_METHOD(TestResize)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);
            dInt.Resize(10);

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");
            dString.Resize(10);
            dString.Resize(7);
            dString.Resize(1);
            
            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);
            dFloat.Resize(10);

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));
            dVector.Resize(10);

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));
            dMatrix.Resize(10);

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);
            dRTTI.Resize(10);
        }

        TEST_METHOD(TestUnknownType)
        {
            Datum dWhatever;
            dWhatever.SetType(Datum::DatumType::Matrix);
            Assert::IsTrue(dWhatever.Type() == Datum::DatumType::Matrix);
            Assert::ExpectException<std::exception>([&dWhatever] { dWhatever.SetType(Datum::DatumType::Vector); });

            Datum dAnother;
            Assert::ExpectException<std::exception>([&dAnother] { dAnother.Resize(20); });
        }
        
        TEST_METHOD(TestToString)
        {
            // ======== INT ===========
            Datum dInt = Datum(Datum::DatumType::Integer);
            dInt.PushBack(1);
            dInt.PushBack(2);
            dInt.PushBack(3);
            dInt.PushBack(4);
            dInt.PushBack(5);
            std::string intString = dInt.ToString();
            dInt.SetFromString(intString, 4);
            Assert::AreEqual(dInt.Front<int>(), dInt.Back<int>());

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            dString.PushBack("one");
            dString.PushBack("two");
            dString.PushBack("three");
            dString.PushBack("four");
            dString.PushBack("five");
            std::string stringString = dString.ToString();
            dString.SetFromString(stringString, 4);
            Assert::AreEqual(dString.Front<std::string>(), dString.Back<std::string>());

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            dFloat.PushBack(1.0f);
            dFloat.PushBack(2.0f);
            dFloat.PushBack(3.0f);
            dFloat.PushBack(4.0f);
            dFloat.PushBack(5.0f);
            std::string floatString = dFloat.ToString();
            dFloat.SetFromString(floatString, 4);
            Assert::AreEqual(dFloat.Front<float>(), dFloat.Back<float>());

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            dVector.PushBack(glm::vec4(1, 1, 1, 1));
            dVector.PushBack(glm::vec4(2, 2, 2, 2));
            dVector.PushBack(glm::vec4(3, 3, 3, 3));
            dVector.PushBack(glm::vec4(4, 4, 4, 4));
            dVector.PushBack(glm::vec4(5, 5, 5, 5));
            std::string vectorString = dVector.ToString();
            dVector.SetFromString(vectorString, 4);
            Assert::AreEqual(dVector.Front<glm::vec4>(), dVector.Back<glm::vec4>());

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            dMatrix.PushBack(glm::mat4(1.0f));
            dMatrix.PushBack(glm::mat4(2.0f));
            dMatrix.PushBack(glm::mat4(3.0f));
            dMatrix.PushBack(glm::mat4(4.0f));
            dMatrix.PushBack(glm::mat4(5.0f));
            std::string matrixString = dMatrix.ToString();
            dMatrix.SetFromString(matrixString, 4);
            Assert::IsTrue(dMatrix.Front<glm::mat4>() == dMatrix.Back<glm::mat4>());

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            dRTTI.PushBack(&a);
            dRTTI.PushBack(&b);
            dRTTI.PushBack(&c);
            dRTTI.PushBack(&d);
            dRTTI.PushBack(&e);
            std::string rttiString = dRTTI.ToString();
            Assert::ExpectException<std::exception>([&dRTTI, &rttiString] { dRTTI.SetFromString(rttiString, 4); });


            Datum dInt2 = Datum(Datum::DatumType::Integer);
            dInt2.PushBackFromString(intString);

            Datum dString2 = Datum(Datum::DatumType::String);
            dString2.PushBackFromString(stringString);

            Datum dFloat2 = Datum(Datum::DatumType::Float);
            dFloat2.PushBackFromString(floatString);

            Datum dVector2 = Datum(Datum::DatumType::Vector);
            dVector2.PushBackFromString(vectorString);

            Datum dMatrix2 = Datum(Datum::DatumType::Matrix);
            dMatrix2.PushBackFromString(matrixString);

            Datum dRTTI2 = Datum(Datum::DatumType::RTTI);
            Assert::ExpectException<std::exception>([&dRTTI2, &rttiString] { dRTTI2.PushBackFromString(rttiString); });
        }

        TEST_METHOD(TestSetStorage)
        {
            // ======== INT ===========
            Datum dInt(Datum::DatumType::Integer);
            int intArray[] = { 1, 2, 3, 4, 5 };
            dInt.SetStorage(intArray, 5);
            Assert::AreEqual(dInt.Get<int>(), 1);

            // ======== STRING ========
            Datum dString = Datum(Datum::DatumType::String);
            std::string stringArray[] = { "one", "two", "three", "four", "five" };
            dString.SetStorage(stringArray, 5);
            Assert::AreEqual(dString.Get<std::string>(), "one"s);

            Assert::ExpectException<std::exception>([&dString] { dString.PushBack("yee"s); });
            Assert::ExpectException<std::exception>([&dString] { std::string a = "a";  dString.PushBack(a); });

            // ======== FLOAT =========
            Datum dFloat = Datum(Datum::DatumType::Float);
            float floatArray[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
            dFloat.SetStorage(floatArray, 5);
            Assert::AreEqual(dFloat.Get<float>(), 1.0f);

            Assert::ExpectException<std::exception>([&dFloat] { dFloat.PushBack(10.0f); });

            // ======== VECTOR ========
            Datum dVector = Datum(Datum::DatumType::Vector);
            glm::vec4 vectorArray[] = { glm::vec4(1, 1, 1, 1) , glm::vec4(2, 2, 2, 2) , glm::vec4(3, 3, 3, 3) };
            dVector.SetStorage(vectorArray, 3);
            Assert::AreEqual(dVector.Get<glm::vec4>(), glm::vec4(1, 1, 1, 1));

            Assert::ExpectException<std::exception>([&dVector] { dVector.PushBack(glm::vec4(1,1,1,1)); });

            // ======== MATRIX ========
            Datum dMatrix = Datum(Datum::DatumType::Matrix);
            glm::mat4 matrixArray[] = { glm::mat4(1.0f) , glm::mat4(2.0f) , glm::mat4(3.0f) };
            dMatrix.SetStorage(matrixArray, 3);
            Assert::IsTrue(dMatrix.Get<glm::mat4>() == glm::mat4(1.0f));

            Assert::ExpectException<std::exception>([&dMatrix] { dMatrix.PushBack(glm::mat4(1.0f)); });

            // ======== RTTI ==========
            Datum dRTTI = Datum(Datum::DatumType::RTTI);
            Foo a(1), b(2), c(3), d(4), e(5), f(6);
            RTTI* RTTIArray[] = { &a, &b, &c, &d };
            dRTTI.SetStorage(RTTIArray, 4);
            Assert::IsTrue(dRTTI.Get<RTTI*>() == &a);

            Assert::ExpectException<std::exception>([&dRTTI, &a] { dRTTI.PushBack(&a); });

            // Blocked Functions
            Assert::ExpectException<std::exception>([&dInt] { dInt.PushBack(100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.RemoveAt(1); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.PopBack(); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Resize(100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Reserve(100); });
            Assert::ExpectException<std::exception>([&dInt] { dInt.Clear(); });

            // Wrong type
            Assert::ExpectException<std::exception>([&dString, &intArray] { dString.SetStorage(intArray, 5); });
            Assert::ExpectException<std::exception>([&dString, &floatArray] { dString.SetStorage(floatArray, 5); });
            Assert::ExpectException<std::exception>([&dString, &vectorArray] { dString.SetStorage(vectorArray, 3); });
            Assert::ExpectException<std::exception>([&dString, &matrixArray] { dString.SetStorage(matrixArray, 3); });
            Assert::ExpectException<std::exception>([&dString, &RTTIArray] { dString.SetStorage(RTTIArray, 4); });
            Assert::ExpectException<std::exception>([&dInt, &stringArray] { dInt.SetStorage(stringArray, 5); });

            // Scalar assignment with empty external datums
            Datum dIntEmpty(Datum::DatumType::Integer);
            dIntEmpty.SetStorage(intArray, 0);

            Datum dStringEmpty = Datum(Datum::DatumType::String);
            dStringEmpty.SetStorage(stringArray, 0);

            Datum dFloatEmpty = Datum(Datum::DatumType::Float);
            dFloatEmpty.SetStorage(floatArray, 0);

            Datum dVectorEmpty = Datum(Datum::DatumType::Vector);
            dVectorEmpty.SetStorage(vectorArray, 0);

            Datum dMatrixEmpty = Datum(Datum::DatumType::Matrix);
            dMatrixEmpty.SetStorage(matrixArray, 0);

            Datum dRTTIEmpty = Datum(Datum::DatumType::RTTI);
            dRTTIEmpty.SetStorage(RTTIArray, 0);

            Assert::ExpectException<std::exception>([&dIntEmpty] { dIntEmpty = 5; });
            Assert::ExpectException<std::exception>([&dStringEmpty] { dStringEmpty = "test"s; });
            Assert::ExpectException<std::exception>([&dFloatEmpty] { dFloatEmpty = 5.0f; });
            Assert::ExpectException<std::exception>([&dVectorEmpty] { dVectorEmpty = glm::vec4(1, 1, 1, 1); });
            Assert::ExpectException<std::exception>([&dMatrixEmpty] { dMatrixEmpty = glm::mat4(1.0f); });
            Assert::ExpectException<std::exception>([&dRTTIEmpty, &a] { dRTTIEmpty = &a; });

            // Already has internal memory
            Datum dInt2(Datum::DatumType::Integer);
            dInt2.Reserve(10);
            Assert::ExpectException<std::exception>([&dInt2, &intArray] { dInt2.SetStorage(intArray, 5); });

            a.QueryInterface(RTTI::TypeIdClass());
            a.Is(RTTI::TypeIdClass());
            a.Is("test");
        }

    private:

        static _CrtMemState sStartMemState;


    };

    _CrtMemState DatumTests::sStartMemState;
}
