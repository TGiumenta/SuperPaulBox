#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include "Scope.h"
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

    template<>
    std::wstring ToString<Scope>(Scope* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<Datum>(Datum* t)
    {
        RETURN_WIDE_STRING(t);
    }

    template<>
    std::wstring ToString<Datum>(const Datum& t)
    {
        std::string str = (const_cast<Datum&>(t)).ToString();
        std::wstring wstr(str.length(), L' ');
        std::copy(str.begin(), str.end(), wstr.begin());
        return wstr;
    }
}

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(ScopeTests)
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
            Scope defaultScope;
            Scope bigScope(100);
        }

        TEST_METHOD(TestCopyConstructor)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;

            Scope& aScope = scope.AppendScope(a);
            Scope& bScope = scope.AppendScope(b); bScope;
            Scope& cScope = aScope.AppendScope(c); cScope;
            Datum& aDatum = cScope.Append("bleh"s); aDatum;
            aDatum = 5;

            Scope copyScope(scope);

            Assert::IsTrue(copyScope == scope);

            Scope& extraScope = cScope.AppendScope(b);
            extraScope;

            Assert::IsFalse(copyScope == scope);
        }

        TEST_METHOD(TestAssignmentOperator)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;

            Scope& aScope = scope.AppendScope(a);
            Scope& bScope = scope.AppendScope(b); bScope;
            Scope& cScope = aScope.AppendScope(c); cScope;
            Datum& aDatum = scope.Append("bleh"s); aDatum;
            aDatum = 5;

            Scope scope2;

            Scope& aScope2 = scope2.AppendScope(b);
            Scope& bScope2 = aScope2.AppendScope(b); bScope2;
            Scope& cScope2 = bScope2.AppendScope(c); cScope2;

            Assert::IsTrue(scope != scope2);
            scope2 = scope;
            Assert::IsTrue(scope == scope2);
        }

        TEST_METHOD(TestMoveSemantics)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;
            std::string test1 = "test1";
            std::string test2 = "test2";

            Scope& aScope = scope.AppendScope(a);
            Scope& bScope = scope.AppendScope(b); bScope;
            Scope& cScope = aScope.AppendScope(c); cScope;

            Datum& aDatum = cScope.Append(test1); aDatum;
            Datum& bDatum = bScope.Append(test2); bDatum;

            Scope* movedScope = new Scope(std::move(aScope)); movedScope;
            delete &aScope;

            Assert::AreEqual(cScope.GetParent(), movedScope);

            Scope scope2;
            Scope& aScope2 = scope2.AppendScope(a);
            Scope& bScope2 = scope2.AppendScope(b); bScope2;
            Scope& cScope2 = aScope2.AppendScope("yee"s); cScope2;

            aScope2 = std::move(*movedScope);
            delete movedScope;
        }

        TEST_METHOD(TestAppend)
        {
            using std::string;

            Scope scope;

            string a = "A"s;
            string b = "B"s;
            string c = "C"s;

            Datum& aDatum = scope.Append(a);
            Assert::AreEqual(scope.Size(), size_t(1));
            Assert::IsTrue(&aDatum == scope.Find(a));
            Assert::AreSame(aDatum, scope[0]);
            Assert::AreSame(aDatum, scope[a]);
        }

        TEST_METHOD(TestAppendScope)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;

            Scope& aScope = scope.AppendScope(a);
            Assert::AreEqual(size_t(1), scope.Size());
            Assert::AreEqual(&scope, aScope.GetParent());
            Assert::IsTrue(scope.Equals(aScope.GetParent()));

            // Grandchild
            Scope& bScope = aScope.AppendScope(b);
            bScope;

            Datum* aScopeDatum = scope.Find(a);
            Assert::IsNotNull(aScopeDatum);
            Assert::IsTrue(Datum::DatumType::Table == aScopeDatum->Type());

            // Trying to append scope onto non scope datum
            Datum& nonScopeDatum = aScope.Append("bleh"s);
            nonScopeDatum = "hello"s;
            Assert::ExpectException<std::exception>([&aScope] { aScope.AppendScope("bleh"s); });
        }

        TEST_METHOD(TestEquality)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;

            Scope& aScope = scope.AppendScope(a);
            Assert::IsNotNull(aScope.GetParent());
            Assert::IsTrue(scope == *aScope.GetParent());
            Assert::IsFalse(scope == aScope);

            Foo aFoo;
            Assert::IsFalse(scope.Equals(&aFoo));

            Foo* fooPtr = nullptr;
            Assert::IsFalse(scope.Equals(fooPtr));
        }

        TEST_METHOD(TestAdopt)
        {
            Scope* child = new Scope;
            Assert::IsNull(child->GetParent());

            Scope scope;
            scope.Adopt(*child, "A"s);
            Assert::AreEqual(&scope, child->GetParent());

            Assert::ExpectException<std::exception>([&scope] { scope.Adopt(scope, "A"s); });
            Assert::ExpectException<std::exception>([&scope, &child] { child->Adopt(scope, "A"s); });

            child->Orphan();
            scope.Append("NonTableDatum") = 10;
            Assert::ExpectException<std::exception>([&scope, child] { scope.Adopt(*child, ""s); });
            Assert::ExpectException<std::exception>([&scope, child] { scope.Adopt(*child, "NonTableDatum"s); });
            delete child;

            // Adopting something that already has a parent
            Scope scope2;
            Scope& anotherChild = scope2.AppendScope("scope"s);

            scope.Adopt(anotherChild, "yee"s);
        }

        TEST_METHOD(TestSearch)
        {
            Scope rootScope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;
            std::string d = "D"s;
            std::string e = "E"s;
            std::string f = "F"s;
            std::string target = "Target"s;

            Scope& levelOneScope = rootScope.AppendScope(a);
            Scope& levelTwoScope = levelOneScope.AppendScope(b);
            Datum& targetDatum = levelTwoScope.Append(target);
            Scope& levelThreeScope = levelTwoScope.AppendScope(c);
            Scope& levelFourScope = levelThreeScope.AppendScope(d);
            const Scope& levelFiveScope = levelFourScope.AppendScope(e);

            auto [foundDatum, foundScope] = levelFourScope.Search(target);
            Assert::AreEqual(foundDatum, &targetDatum);
            Assert::AreEqual(foundScope, &levelTwoScope);

            Datum& closerTarget = levelFourScope.Append(target);
            auto [foundDatum2, foundScope2] = levelFourScope.Search(target);
            Assert::AreEqual(foundDatum2, &closerTarget);
            Assert::AreEqual(foundScope2, &levelFourScope);

            const Scope constScope(rootScope);
            auto [constFoundDatum, constFoundScope] = constScope.Search(target);
            auto [constFoundDatum2, constFoundScope2] = levelFiveScope.Search(target);
            constScope.GetParent();
        }

        TEST_METHOD(TestOrphan)
        {
            Scope scope;
            Scope& aScope = scope.AppendScope("A"s);
            Assert::AreEqual(&scope, aScope.GetParent());

            aScope.Orphan();
            Assert::IsTrue(aScope.GetParent() == nullptr);

            delete& aScope;
        }

        TEST_METHOD(TestFindContainedScope)
        {
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;
            std::string d = "D"s;

            Scope rootScope;
            Scope& aScope = rootScope.AppendScope(a);
            Scope& bScope = rootScope.AppendScope(b); bScope;
            Scope& targetScope = rootScope.AppendScope(b); targetScope;
            Scope& cScope = aScope.AppendScope(c); cScope;

            auto [foundDatum, index] = rootScope.FindContainedScope(targetScope);
            Assert::AreEqual(index, size_t(1));
            Assert::AreEqual(&(foundDatum->Get<Scope>(index)), &targetScope);
            
            Scope uncontainedScope;
            auto [nullDatum, whatever] = rootScope.FindContainedScope(uncontainedScope);
            Assert::IsNull(nullDatum);
        }

        TEST_METHOD(TestAncestorDescendant)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;
            std::string d = "D"s;
            std::string e = "E"s;
            std::string f = "F"s;

            Scope& aScope = scope.AppendScope(a);
            Scope& bScope = scope.AppendScope(b); bScope;
            Scope& cScope = aScope.AppendScope(c); cScope;
            Scope& cScope2 = aScope.AppendScope(c); cScope2;
            Scope& cScope3 = aScope.AppendScope(c); cScope3;
            Scope& dScope = cScope3.AppendScope(d); dScope;
            Scope& eScope = dScope.AppendScope(e); eScope;

            Assert::IsTrue(eScope.IsDescendantOf(cScope3));
            Assert::IsTrue(eScope.IsDescendantOf(scope));
            Assert::IsFalse(scope.IsDescendantOf(cScope3));
            Assert::IsTrue(scope.IsAncestorOf(cScope2));
            Assert::IsTrue(scope.IsAncestorOf(bScope));
            Assert::IsTrue(scope.IsAncestorOf(eScope));
            Assert::IsFalse(eScope.IsDescendantOf(bScope));
        }

        TEST_METHOD(TestDatumMethods)
        {
            Scope scope;
            std::string a = "A"s;
            std::string b = "B"s;
            std::string c = "C"s;

            Scope& aScope = scope.AppendScope(a);
            Scope& bScope = scope.AppendScope(b); bScope;
            Scope& cScope = aScope.AppendScope(c); cScope;
            Scope& cScope2 = aScope.AppendScope(c); cScope2;
            Scope& cScope3 = aScope.AppendScope(c); cScope3;
            
            Datum* cScopeDatum = aScope.Find(c);
            Assert::IsTrue(cScopeDatum->Front<Scope>() == cScope);
            Assert::IsTrue(cScopeDatum->Back<Scope>() == cScope3);

            const Scope aConstScope(aScope);
            const Datum* cConstScopeDatum = aConstScope.Find(c);
            Assert::IsTrue(cConstScopeDatum->Front<Scope>() == cScope);
            Assert::IsTrue(cConstScopeDatum->Back<Scope>() == cScope3);

            Assert::ExpectException<std::exception>([&cScopeDatum] { Scope* s = nullptr;  cScopeDatum->Set(s); });
            Assert::ExpectException<std::exception>([&cScopeDatum] { cScopeDatum->Resize(5); });
            Assert::ExpectException<std::exception>([&cScopeDatum] { cScopeDatum->Get<Scope>(100); });
            Assert::ExpectException<std::exception>([&cScopeDatum, &cScope3] { cScopeDatum->Set(&cScope3, 100); });
            Assert::ExpectException<std::exception>([&cConstScopeDatum] { cConstScopeDatum->Get<Scope>(100); });

            Datum datum;
            datum = 1;
            Assert::ExpectException<std::exception>([&datum, &cScope3] { datum.Set(&cScope3, 0); });
            Assert::ExpectException<std::exception>([&datum, &cScope3] { datum = &cScope3; });
            Assert::ExpectException<std::exception>([&datum, &cScope3] { datum = cScope3; });

            Datum unknownDatum;
            unknownDatum = &cScope3;
            unknownDatum = &cScope2;

            Scope& test = (*cScopeDatum)[0]; test;

            Datum anotherDatum;
            anotherDatum = aScope;
            anotherDatum = cScope2;
        }


    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState ScopeTests::sStartMemState;
}
