#include "pch.h"
#include <CppUnitTest.h>
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "Power.h"
#include "Entity.h"
#include "Avatar.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "ActionDestroyAction.h"
#include "ActionCreateAction.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestLibraryDesktop
{
    TEST_MODULE_INITIALIZE(TestModuleInitialize)
    {
        RegisterType(AttributedFoo, Attributed);
        RegisterType(Power, Attributed);
        RegisterType(Entity, Attributed);
        RegisterType(Avatar, Entity);
        RegisterType(Action, Attributed);
        RegisterType(ActionList, Action);
        RegisterType(ActionIncrement, Action);
        RegisterType(ActionListIf, Action);
        RegisterType(ActionDestroyAction, Action);
        RegisterType(ActionCreateAction, Action);
    }

    TEST_MODULE_CLEANUP(TestModuleCleanup)
    {
        TypeManager::Clear();
    }
}