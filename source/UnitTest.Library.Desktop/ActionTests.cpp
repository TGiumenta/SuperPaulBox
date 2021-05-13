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
#include "ActionIncrement.h"
#include "WorldState.h"
#include "Avatar.h"
#include "JsonTableParseHelper.h"
#include "JsonParseCoordinator.h"
#include "ActionListIf.h"
#include "ActionDestroyAction.h"
#include "ActionCreateAction.h"
#include "ActionList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
    TEST_CLASS(ActionTests)
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

        TEST_METHOD(TestActionIncrement)
        {
            ActionIncrement increment;
            Datum& a = increment.AppendAuxiliaryAttribute("A"s);
            a = 0.0f;
            increment.SetTarget("A"s);
            Assert::AreEqual("A"s, increment.GetTarget());
            Assert::AreEqual(1.0f, increment.GetStep());

            WorldState worldState;

            Assert::AreEqual(0.0f, a.Get<float>());
            increment.Update(&worldState);
            Assert::AreEqual(1.0f, a.Get<float>());
            increment.Update(&worldState);
            Assert::AreEqual(2.0f, a.Get<float>());

            increment.SetStep(2.0f);
            Assert::AreEqual(2.0f, increment.GetStep());
            increment.Update(&worldState);
            Assert::AreEqual(4.0f, a.Get<float>());

            increment.SetTarget("B"s);
            Assert::ExpectException<std::exception>([&increment, &worldState] { increment.Update(&worldState); });
        }

        TEST_METHOD(TestAvatarAction)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;
            const AvatarFactory avatarFactory;
            const ActionIncrementFactory actionIncrementFactory;

            JsonTableParseHelper tableHelper;
            Avatar testAvatar;
            JsonTableParseHelper::SharedData sharedData(testAvatar);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "AvatarActionTest.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));

            WorldState worldState;
            float originalDPS = testAvatar.m_dps;
            testAvatar.Update(&worldState);
            Assert::AreEqual(originalDPS + 1.0f, testAvatar.m_dps);
        }

        TEST_METHOD(TestActionListIf)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;
            const AvatarFactory avatarFactory;
            const ActionIncrementFactory actionIncrementFactory;
            const ActionListIfFactory actionListIfFactory;
            WorldState worldState;
            ActionListIf actionListIf;

            Datum& a = actionListIf.AppendAuxiliaryAttribute("A"s);
            a = 0.0f;
            Datum& b = actionListIf.AppendAuxiliaryAttribute("B"s);
            b = 0.0f;

            ActionIncrement* aIncrement = new ActionIncrement();
            aIncrement->SetTarget("A"s);
            ActionIncrement* bIncrement = new ActionIncrement();
            bIncrement->SetTarget("B"s);

            actionListIf.Adopt(*aIncrement, "Then"s);
            actionListIf.Adopt(*bIncrement, "Else"s);

            {
                actionListIf.SetCondition(true);
                actionListIf.Update(&worldState);
                Assert::AreEqual(1.0f, a.Get<float>());
                Assert::AreEqual(0.0f, b.Get<float>());

                actionListIf.Update(&worldState);
                Assert::AreEqual(2.0f, a.Get<float>());
                Assert::AreEqual(0.0f, b.Get<float>());
            }

            {
                actionListIf.SetCondition(false);
                actionListIf.Update(&worldState);
                Assert::AreEqual(1.0f, b.Get<float>());
                Assert::AreEqual(2.0f, a.Get<float>());

                actionListIf.Update(&worldState);
                Assert::AreEqual(2.0f, b.Get<float>());
                Assert::AreEqual(2.0f, a.Get<float>());
            }

            bIncrement->SetTarget("C"s);
            Assert::ExpectException<std::exception>([&actionListIf, &worldState] { actionListIf.Update(&worldState); });

            ActionListIf* clonedActionListIf = actionListIf.Clone();
            delete clonedActionListIf;

        }

        TEST_METHOD(TestActionListParsing)
        {
            const ScopeFactory scopeFactory;
            const EntityFactory entityFactory;
            const AvatarFactory avatarFactory;
            const ActionIncrementFactory actionIncrementFactory;
            const ActionListIfFactory actionListIfFactory;

            JsonTableParseHelper tableHelper;
            Avatar testAvatar;
            JsonTableParseHelper::SharedData sharedData(testAvatar);
            JsonParseCoordinator jpc(sharedData);
            jpc.AddHelper(tableHelper);

            std::string filename = "ActionListTest.json"s;

            Assert::AreEqual(sharedData.Depth(), size_t(0));
            jpc.ParseFromFile(filename);
            Assert::AreEqual(sharedData.Depth(), size_t(0));

            WorldState worldState;
            float originalDPS = testAvatar.m_dps;
            testAvatar.Update(&worldState);
            Assert::AreEqual(originalDPS + 1.0f, testAvatar.m_dps);

        }

        TEST_METHOD(TestActionDeleteAction)
        {
            WorldState worldState;
            Entity rootEntity;

            // Set up a nested entity
            Entity* nestedEntity = new Entity();
            rootEntity.Adopt(*nestedEntity, "Children"s);

            // Set up an action
            Datum& a = rootEntity.AppendAuxiliaryAttribute("A"s);
            a = 0.0f;
            ActionIncrement* aIncrement = new ActionIncrement();
            aIncrement->SetTarget("A"s);
            aIncrement->SetName("TestIncrement");
            rootEntity.Adopt(*aIncrement, "Actions"s);

            // Set up destroy action
            ActionDestroyAction* destroyAction = new ActionDestroyAction();
            destroyAction->SetTarget("TestIncrement");
            nestedEntity->Adopt(*destroyAction, "Actions"s);

            Assert::AreEqual(0.0f, a.Get<float>());
            rootEntity.Update(&worldState);
            Assert::AreEqual(1.0f, a.Get<float>());

            worldState.CompletePendingRequests();

            rootEntity.Update(&worldState);
            Assert::AreEqual(1.0f, a.Get<float>());

            ActionDestroyAction* clonedActionDestroyAction = destroyAction->Clone();
            delete clonedActionDestroyAction;
        }

        TEST_METHOD(TestActionCreateAction)
        {
            const ActionIncrementFactory actionIncrementFactory;
            const EntityFactory entityFactory;

            WorldState worldState;
            Entity rootEntity;

            // Set up a nested entity
            Entity* nestedEntity = new Entity();
            rootEntity.Adopt(*nestedEntity, "Children"s);

            // Set up a datum
            Datum& a = rootEntity.AppendAuxiliaryAttribute("A"s);
            a = 0.0f;
            
            // Set up create action
            ActionCreateAction* createAction = new ActionCreateAction();
            createAction->SetClassName("ActionIncrement");
            createAction->SetActionName("TestIncrement");
            nestedEntity->Adopt(*createAction, "Actions"s);

            Assert::AreEqual(0.0f, a.Get<float>());
            rootEntity.Update(&worldState);
            Assert::AreEqual(0.0f, a.Get<float>());

            worldState.CompletePendingRequests();

            // Set the new action to have a target of A
            nestedEntity->Find("Actions")->Back<Scope>().As<ActionIncrement>()->SetTarget("A"s);

            rootEntity.Update(&worldState);
            Assert::AreEqual(1.0f, a.Get<float>());

            ActionCreateAction* clonedActionCreateAction = createAction->Clone();
            delete clonedActionCreateAction;

            createAction->SetClassName("bleh");
            rootEntity.Update(&worldState);
            Assert::ExpectException<std::exception>([&worldState] { worldState.CompletePendingRequests(); });

            createAction->SetClassName("Entity");
            rootEntity.Update(&worldState);
            Assert::ExpectException<std::exception>([&worldState] { worldState.CompletePendingRequests(); });

            worldState.SetGameTime(worldState.GetGameTime());
            nestedEntity->CreateAction("ActionIncrement", "test");
            Assert::ExpectException<std::exception>([&nestedEntity] { nestedEntity->CreateAction("Entity", "test"); });
            Assert::ExpectException<std::exception>([&nestedEntity] { nestedEntity->CreateAction("bleh", "test"); });
        }

        TEST_METHOD(TestActionList)
        {
            ActionList actionList;
            ActionList actionList2(ActionList::TypeIdClass());
            WorldState worldState;

            ActionListIf* actionListIf = new ActionListIf();
            actionList.Adopt(*actionListIf, "Actions"s);

            actionList.Update(&worldState);
        }





    private:
        static _CrtMemState sStartMemState;
    };

    _CrtMemState ActionTests::sStartMemState;
}
