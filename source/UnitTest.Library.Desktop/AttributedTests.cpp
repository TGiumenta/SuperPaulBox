#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include "Scope.h"
#include "Datum.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(AttributedTests)
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
            AttributedFoo attFoo;
        }

        TEST_METHOD(TestCopyConstructor)
        {
            // Copy constructor
            AttributedFoo attFoo;
            AttributedFoo attFoo2(attFoo);
            Assert::IsTrue(attFoo.Equals(&attFoo2));

            attFoo.AppendAuxiliaryAttribute("blah");

            Assert::IsFalse(attFoo.Equals(&attFoo2));
        }

        TEST_METHOD(TestMoveConstructor)
        {
            // Move constructor
            AttributedFoo attFoo;
            AttributedFoo attFoo2(attFoo);
            AttributedFoo attFoo3(std::move(attFoo));

            Assert::IsTrue(attFoo2.Equals(&attFoo3));

            // Move assignment
            AttributedFoo attFoo4;
            attFoo4.AppendAuxiliaryAttribute("beep");
            attFoo4.AppendAuxiliaryAttribute("boop");
            attFoo4.AppendAuxiliaryAttribute("bop");

            attFoo4 = std::move(attFoo3);
            Assert::IsTrue(attFoo4.Equals(&attFoo2));
        }

        TEST_METHOD(TestCopyAssignment)
        {
            AttributedFoo attFoo;
            attFoo.AppendAuxiliaryAttribute("blah");

            // Copy assignment
            AttributedFoo attFoo3;
            attFoo3.AppendAuxiliaryAttribute("beep");
            attFoo3.AppendAuxiliaryAttribute("boop");
            attFoo3.AppendAuxiliaryAttribute("bop");

            attFoo3 = attFoo;
            Assert::IsTrue(attFoo3.Equals(&attFoo));
        }

        TEST_METHOD(TestMoveAssignment)
        {
            // Move constructor
            AttributedFoo attFoo;
            AttributedFoo attFoo2(attFoo);

            // Move assignment
            AttributedFoo attFoo4;
            attFoo4.AppendAuxiliaryAttribute("beep");
            attFoo4.AppendAuxiliaryAttribute("boop");
            attFoo4.AppendAuxiliaryAttribute("bop");

            attFoo4 = std::move(attFoo);
            Assert::IsTrue(attFoo4.Equals(&attFoo2));
        }

        TEST_METHOD(TestAppendAuxiliary)
        {
            AttributedFoo attFoo;

            size_t originalSize = attFoo.Size();
            attFoo.AppendAuxiliaryAttribute("blah");
            Assert::AreEqual(attFoo.Size(), size_t(originalSize + 1));
            
            Assert::ExpectException<std::exception>([&attFoo] { attFoo.AppendAuxiliaryAttribute("ExternalInteger"); });
        }

        TEST_METHOD(TestAccessors)
        {
            AttributedFoo attFoo;
            attFoo.AppendAuxiliaryAttribute("beep");
            attFoo.AppendAuxiliaryAttribute("boop");
            attFoo.AppendAuxiliaryAttribute("bop");

            auto& attributes =  attFoo.GetAttributes();
            for (Scope::PairType* pairPtr : attributes)
            {
                Assert::IsTrue(attFoo.IsAttribute(pairPtr->first));
            }

            auto prescribedAttributes = attFoo.GetPrescribedAttributes();
            for (Scope::PairType* pairPtr : prescribedAttributes)
            {
                Assert::IsTrue(attFoo.IsPrescribedAttribute(pairPtr->first));
                Assert::IsFalse(attFoo.IsAuxiliaryAttribute(pairPtr->first));
            }

            auto auxiliaryAttributes = attFoo.GetAuxiliaryAttributes();
            for (Scope::PairType* pairPtr : auxiliaryAttributes)
            {
                Assert::IsTrue(attFoo.IsAuxiliaryAttribute(pairPtr->first));
                Assert::IsFalse(attFoo.IsPrescribedAttribute(pairPtr->first));
            }
        }


    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState AttributedTests::sStartMemState;
}
