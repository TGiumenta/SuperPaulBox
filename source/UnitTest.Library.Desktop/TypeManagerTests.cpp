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
    class Monster : public RTTI
    {
        RTTI_DECLARATIONS(Monster, RTTI);

    public:
        std::string m_name;
        int m_hitPoints;
        float m_dps;

        static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures()
        {
            return Vector<Signature>
            {
                { "Name", Datum::DatumType::String, 1, offsetof(Monster, m_name)},
                { "HitPoints", Datum::DatumType::Integer, 1, offsetof(Monster, m_hitPoints) },
                { "Dps", Datum::DatumType::Float, 1, offsetof(Monster, m_dps) }
            };
        };
    };

    RTTI_DEFINITIONS(Monster);

    class MeanMonster : public Monster
    {
        RTTI_DECLARATIONS(MeanMonster, Monster);

    public:
        int m_grumpiness;

        static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures()
        {
            return Vector<Signature>
            {
                { "Grumpiness", Datum::DatumType::Integer, 1, offsetof(MeanMonster, m_grumpiness)}
            };
        };
    };

    RTTI_DEFINITIONS(MeanMonster);

    TEST_CLASS(TypeManagerTests)
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

        TEST_METHOD(TestTypeManager)
        {
            size_t startingSize = TypeManager::Size();

            // Trying to add Monster with itself as a parent
            Assert::ExpectException<std::exception>([] { TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures(), Monster::TypeIdClass()); });

            Assert::IsFalse(TypeManager::ContainsType(Monster::TypeIdClass()));
            TypeManager::AddType(Monster::TypeIdClass(), Monster::Signatures(), RTTI::TypeIdClass());
            Assert::AreEqual(TypeManager::Size(), size_t(startingSize + 1));

            Assert::IsFalse(TypeManager::ContainsType(MeanMonster::TypeIdClass()));
            TypeManager::AddType(MeanMonster::TypeIdClass(), MeanMonster::Signatures(), Monster::TypeIdClass());
            Assert::AreEqual(TypeManager::Size(), size_t(startingSize + 2));

            auto meanMonsterSignatures = TypeManager::GetSignatures(MeanMonster::TypeIdClass());
            Assert::AreEqual(meanMonsterSignatures.Size(), size_t(4));

            Assert::IsTrue(TypeManager::ContainsType(Monster::TypeIdClass()));
            Assert::IsTrue(TypeManager::ContainsType(MeanMonster::TypeIdClass()));

            TypeManager::RemoveType(MeanMonster::TypeIdClass());
            Assert::AreEqual(TypeManager::Size(), size_t(startingSize + 1));

            TypeManager::RemoveType(Monster::TypeIdClass());
            Assert::AreEqual(TypeManager::Size(), startingSize);

            // Trying to get signatures for not registered type
            Assert::ExpectException<std::exception>([] { TypeManager::GetSignatures(Monster::TypeIdClass()); });
        }



    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState TypeManagerTests::sStartMemState;
}
