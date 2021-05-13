#include "pch.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "RenderUtil.h"
#include "Entity.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "WorldState.h"
#include "TypeManager.h"
#include "PhysicsEntity.h"
#include "Animation.h"
#include <glm/glm.hpp>
#include "VertexDeclarations.h"
#include "ShaderUtil.h"
#include "ShaderProgram.h"
#include "OrthographicCamera.h"
#include <exception>
#include "Game.h"
#include "Player.h"
#include "Weapon.h"
#include "Pistol.h"
#include "MachineGun.h"
#include "Revolver.h"
#include "Shotgun.h"
#include "Projectile.h"
#include "Number.h"
#include "LevelGenerator.h"
#include "ScoreManager.h"
#include "CrateManager.h"
#include "Crate.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "ActionIncrement.h"
#include "EventMessageAttributed.h"
#include "CustomGame.h"
#include "AudioManager.h"

using namespace FieaGameEngine;

// Place all types we need to register here
void InitTypes()
{
    RegisterType(Entity, Attributed);
	RegisterType(PhysicsEntity, Entity);
    RegisterType(Animation, Attributed);
    RegisterType(Player, PhysicsEntity);
    RegisterType(Weapon, Entity);
    RegisterType(Pistol, Weapon);
    RegisterType(MachineGun, Weapon);
    RegisterType(Revolver, Weapon);
    RegisterType(Shotgun, Weapon);
    RegisterType(Projectile, PhysicsEntity);
	RegisterType(Number, Entity);
	RegisterType(ScoreManager, Entity);
    RegisterType(Crate, PhysicsEntity);
    RegisterType(Enemy, PhysicsEntity);
    RegisterType(CrateManager, Entity);
    RegisterType(EnemyManager, Entity);
	RegisterType(ReactionAttributed, Reaction);
	RegisterType(ActionIncrement, Action);
	RegisterType(EventMessageAttributed, Attributed);
}

int WINAPI WinMain(HINSTANCE /*instance*/, HINSTANCE /*previousInstance*/, LPSTR /*commandLine*/, int /*showCommand*/)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    // Factories
    const ScopeFactory scopeFactory;
    const EntityFactory entityFactory;
    const PhysicsEntityFactory physicsEntityFactory;
    const AnimationFactory animationFactory;
    const PlayerFactory playerFactory;
    const PistolFactory pistolFactory;
    const MachineGunFactory machineGunFactory;
    const RevolverFactory revolverFactory;
    const ShotgunFactory shotgunFactory;
    const ProjectileFactory projectileFactory;
	const NumberFactory numberFactory;
	const ScoreManagerFactory scoreManagerFactory;
    const CrateManagerFactory crateManagerFactory;
    const CrateFactory crateFactory;
    const EnemyFactory enemyFactory;
    const EnemyManagerFactory enemyManagerFactory;
	const ReactionAttributedFactory reactionAttributedFactory;
	const ActionIncrementFactory actionIncrementFactory;

    srand(uint32_t(time(NULL)));

    InitTypes();

    CustomGame::Create();

    if (!Game::Initialize())
    {
        return EXIT_FAILURE;
    }

    //Game::GetGame()->LoadWorldFromJSON("ExampleWorld.json");
    CustomGame::GetCustomGame()->SetMainMenuImage("MainMenu.png");
    CustomGame::GetCustomGame()->SetInstructionsImage("Instructions.png");
    CustomGame::GetCustomGame()->SetGameOverImage("GameOverScreen.png");

    //Game::GetGame()->LoadWorldFromJSON("ExampleWorld.json");

    Game::GetGame()->Run();

    Game::Shutdown();

	return EXIT_SUCCESS;
}


