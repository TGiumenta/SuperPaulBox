#pragma once
#include "WorldState.h"
#include "Entity.h"
#include "WorldContactListener.h"
#include <box2d/box2d.h>
#include "Input.h"
#include "Event.h"

namespace FieaGameEngine
{
    

	class Game
	{
	public:
		static Game* GetGame();
		static bool Initialize();
		static void Shutdown();

		void LoadWorldFromJSON(const std::string& filename);
		virtual void Run() = 0;

		

		inline WorldState& GetWorldState() { return m_worldState; }
		inline GLFWwindow* GetWindow() { return m_window; }
		inline int GetScreenWidth() const { return s_screenWidth; }
		inline int GetScreenHeight() const { return s_screenHeight; }
		inline b2World& GetPhysicsWorld() { return *m_physicsWorld; }
		Entity& GetRootEntity();
		
		float m_totalGameTime = 0;
		
	protected:
		inline static Game* s_gameInstance;
		inline static const int s_screenWidth = 1280;
		inline static const int s_screenHeight = 720;
		inline static std::string s_gameTitle = "Super Paul Box";

		GLFWwindow* m_window;
		Entity m_rootEntity;
		WorldState m_worldState;

		GameClock m_gameClock;
		b2World* m_physicsWorld = nullptr;
		

		Game() = default;
		virtual ~Game() = default;

	private:
        
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;
        Game(Game&&) = delete;
        Game& operator=(Game&&) = delete;
         
	};
}

