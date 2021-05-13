#include "pch.h"
#include "Game.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"

namespace FieaGameEngine
{
    Game* Game::GetGame()
    {
        assert(s_gameInstance != nullptr);
        return s_gameInstance;
    }

    bool Game::Initialize()
    {
        //s_gameInstance = new Game();

        assert(s_gameInstance);

        const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

        if (!glfwInit())
        {
            return false;
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        s_gameInstance->m_window = glfwCreateWindow(s_screenWidth, s_screenHeight, s_gameTitle.c_str(), nullptr, nullptr);
        if (s_gameInstance->m_window == nullptr)
        {
            return false;
        }

        glfwMakeContextCurrent(s_gameInstance->m_window);

        if (gl3wInit() != 0)
        {
            return false;
        }

        glViewport(0, 0, s_screenWidth, s_screenHeight);
        
        RenderUtil::Init();
        InputManager::Inititalize(s_gameInstance->m_window);

        return true;
    }

    void Game::Shutdown()
    {
        // Clear the root entity first to make sure it is destroyed before the physics world
        s_gameInstance->m_rootEntity.Clear();

        glfwDestroyWindow(s_gameInstance->m_window);

        delete s_gameInstance;
        
        RenderUtil::Shutdown();
        
        glfwTerminate();
    }

    void Game::LoadWorldFromJSON(const std::string& filename)
    {
        JsonTableParseHelper tableHelper;
        JsonTableParseHelper::SharedData sharedData(m_rootEntity);
        JsonParseCoordinator jpc(sharedData);
        jpc.AddHelper(tableHelper);

        jpc.ParseFromFile(filename);

        m_rootEntity.Init(&m_worldState);
    }

    /*void Game::Run()
    {
        float timeStep = 1.0f / 60.f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        float timeUnconsumed = 0.0f;

        const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
        while (!glfwWindowShouldClose(m_window))
        {
            InputManager::Update();

            glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
            m_gameClock.UpdateGameTime(m_worldState.GetGameTime());

            float deltaTime = m_worldState.GetDeltaTime();
            for (timeUnconsumed += deltaTime; timeUnconsumed >= timeStep; timeUnconsumed -= timeStep)
            {
                m_physicsWorld.Step(timeStep, velocityIterations, positionIterations);
            }

            m_totalGameTime += deltaTime;
            m_rootEntity.Update(&m_worldState);
            m_worldState.CompletePendingRequests();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);


            m_rootEntity.Render(&m_worldState);

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }*/

    Entity& Game::GetRootEntity()
    {
        return m_rootEntity;
    }

}
