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
#include "Avatar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(JsonParseCoordinatorTests)
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
            JsonTestParseHelper testHelper;
            JsonTestParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(testHelper);

            Assert::IsTrue(&sharedData == jpc.GetSharedData());

            JsonTestParseHelper::SharedData newSharedData;
            jpc.SetSharedData(&newSharedData);
            Assert::IsTrue(&newSharedData == jpc.GetSharedData());
            Assert::IsTrue(&jpc == newSharedData.GetJsonParseCoordinator());
        }

        TEST_METHOD(TestTableParser)
        {
            const ScopeFactory scopeFactory;

            JsonTableParseHelper tableHelper;
            Scope testScope;
            JsonTableParseHelper::SharedData sharedData(testScope);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "TestScope.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));

            JsonParseCoordinator* copyJpc = jpc.Clone();
            delete copyJpc;
        }

        TEST_METHOD(TestTableArrays)
        {
            const ScopeFactory scopeFactory;
            const PowerFactory powerFactory;

            JsonTableParseHelper tableHelper;
            Scope testScope;
            JsonTableParseHelper::SharedData sharedData(testScope);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "TestArrays.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));
        }

        TEST_METHOD(TestFileParsing)
        {
            JsonTestParseHelper testHelper;
            JsonTestParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(testHelper);

            std::string filename = "Integer.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));

            Assert::AreEqual(filename, jpc.GetFileName());

            Assert::ExpectException<std::exception>([&jpc] { jpc.ParseFromFile("badfilename"s); });
        }

        TEST_METHOD(TestStreamParsing)
        {
            JsonIntegerParseHelper integerParseHelper;
            integerParseHelper.Initialize();
            JsonTestParseHelper testHelper;
            JsonTestParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(integerParseHelper);
            jpc.AddHelper(testHelper);

            std::string filename = "Integer.json"s;
            std::ifstream inputFile(filename, std::ifstream::binary);

            jpc.Parse(inputFile);
        }

        TEST_METHOD(TestBadClass)
        {
            const ScopeFactory scopeFactory;
            const PowerFactory powerFactory;

            JsonTableParseHelper tableHelper;
            Scope testScope;
            JsonTableParseHelper::SharedData sharedData(testScope);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string inputString = R"({"Address": {
                                            "type": "table",
                                            "class": "thisclassdoesntexist",
                                            "value": {
                                                "Street": {
                                                    "type": "string",
                                                    "value": "123 Anystreet St."
                                                }
                                            }
                                         }})";

            Assert::ExpectException<std::exception>([&jpc, &inputString] { jpc.Parse(inputString); });
            
        }

        TEST_METHOD(TestIntegerParsing)
        {
            std::string inputString = R"({"integer" : 100})";

            JsonIntegerParseHelper integerParseHelper;
            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);

            jpc.AddHelper(integerParseHelper);
            jpc.Parse(inputString);

            Assert::AreEqual(sharedData.m_data.Size(), size_t(1));
            Assert::AreEqual(sharedData.m_data[0], 100);
        }

        TEST_METHOD(TestClone)
        {
            JsonIntegerParseHelper integerParseHelper;
            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(integerParseHelper);

            JsonParseCoordinator* copyJpc = jpc.Clone();
            delete copyJpc;
        }

        TEST_METHOD(TestAddRemoveHelpers)
        {
            JsonIntegerParseHelper integerParseHelper;
            JsonTestParseHelper testHelper;

            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);

            jpc.AddHelper(integerParseHelper);
            jpc.AddHelper(testHelper);

            jpc.RemoveHelper(integerParseHelper);

            // Adding and removing helper from clone
            JsonParseCoordinator* copyJpc = jpc.Clone();

            Assert::ExpectException<std::exception>([&copyJpc, &testHelper] { copyJpc->AddHelper(testHelper); });
            Assert::ExpectException<std::exception>([&copyJpc, &testHelper] { copyJpc->RemoveHelper(testHelper); });
            delete copyJpc;

            
        }

        TEST_METHOD(TestMoveConstructor)
        {
            std::string inputString = R"({"integer" : 100})";

            JsonIntegerParseHelper integerParseHelper;
            JsonTableParseHelper tableHelper;
            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);
            jpc.AddHelper(integerParseHelper);
            

            JsonParseCoordinator movedJpc(std::move(jpc));

            movedJpc.Parse(inputString);

            Assert::AreEqual(sharedData.m_data.Size(), size_t(1));
            Assert::AreEqual(sharedData.m_data[0], 100);

            tableHelper.Is(JsonTableParseHelper::TypeIdClass());
        }

        TEST_METHOD(TestMoveAssignment)
        {
            std::string inputString = R"({"integer" : 100})";

            JsonIntegerParseHelper integerParseHelper;
            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(integerParseHelper);

            JsonParseCoordinator movedJpc(sharedData);
            movedJpc = std::move(jpc);

            movedJpc.Parse(inputString);

            Assert::AreEqual(sharedData.m_data.Size(), size_t(1));
            Assert::AreEqual(sharedData.m_data[0], 100);
        }

        TEST_METHOD(TestRTTI)
        {
            JsonIntegerParseHelper integerParseHelper;
            JsonIntegerParseHelper::SharedData sharedData;
            JsonParseCoordinator jpc(sharedData);

            jpc.Is(JsonParseCoordinator::TypeIdClass());
        }

        TEST_METHOD(TestNestedFiles)
        {
            const ScopeFactory scopeFactory;
            const PowerFactory powerFactory;
            const AvatarFactory avatarFactory;

            JsonTableParseHelper tableHelper;
            Scope testScope;
            JsonTableParseHelper::SharedData sharedData(testScope);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            {
                std::ifstream inputFile("TestNestedFile.json"s, std::ifstream::binary);
                if (!inputFile.good())
                {
                    throw std::exception("Failed to read file!");
                }
                Json::Value jsonVal;
                inputFile >> jsonVal;

                jpc.ParseNestedFiles(jsonVal);

                std::string testString = jsonVal.toStyledString();
            }

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile("TestNestedFile.json"s);
            Assert::AreEqual(sharedData.Depth(), size_t(0));
        }
        


    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState JsonParseCoordinatorTests::sStartMemState;
}
