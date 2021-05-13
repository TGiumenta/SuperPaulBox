#include "pch.h"
#include <crtdbg.h>
#include <CppUnitTest.h>
#include "foo.h"
#include "Scope.h"
#include "Datum.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include <exception>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include "JsonParseCoordinator.h"
#include "JsonIntegerParseHelper.h"
#include "IJsonParseHelper.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "Power.h"
#include "Entity.h"
#include "Avatar.h"
#include "WorldState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(EntityTests)
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
            Entity entity; entity;
            int i = 0; i;
        }

        TEST_METHOD(TestParseEntity)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;

            JsonTableParseHelper tableHelper;
            Entity testSector;
            JsonTableParseHelper::SharedData sharedData(testSector);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "EntityTest.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));
        }

        TEST_METHOD(TestParseWorld)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;
            const AvatarFactory avatarFactory;

            JsonTableParseHelper tableHelper;
            Entity testWorld;
            JsonTableParseHelper::SharedData sharedData(testWorld);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "WorldTest.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));
        }

        TEST_METHOD(TestMiscFunctions)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;
            const AvatarFactory avatarFactory;

            JsonTableParseHelper tableHelper;
            Entity testWorld;
            JsonTableParseHelper::SharedData sharedData(testWorld);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "WorldTest.json"s;
            jpc.ParseFromFile(filename);

            WorldState worldState;
            testWorld.Update(&worldState);

            Scope* clonedEntity = testWorld.Clone();
            delete clonedEntity;
        }

        

    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState EntityTests::sStartMemState;
}
