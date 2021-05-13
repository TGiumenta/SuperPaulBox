#include "pch.h"
#include "CustomGame.h"
#include "RenderUtil.h"
#include "LevelGenerator.h"

void CustomGame::Create()
{
    s_gameInstance = s_customGameInstance = new CustomGame();
}

CustomGame* CustomGame::GetCustomGame()
{
    assert(s_customGameInstance != nullptr);
    return s_customGameInstance;
}

void CustomGame::Run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        InputManager::Update();

        CustomGame::UpdateFunction func = m_UpdateFunctions.at(m_currentState);
        (this->*func)();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void CustomGame::SetMainMenuImage(const std::string& filename)
{
    m_MenuTexture = RenderUtil::LoadTexture(filename);
}

void CustomGame::SetInstructionsImage(const std::string& filename)
{
    m_InstructionsTexture = RenderUtil::LoadTexture(filename);
}

void CustomGame::SetGameOverImage(const std::string& filename)
{
    m_GameOverTexture = RenderUtil::LoadTexture(filename);
}

#pragma region State Changes
void CustomGame::ChangeState(GameState newState)
{
    m_currentState = newState;

    if (newState == GameState::GAME_MENU)
    {
		AudioManager::PlaySoundEvent playSoundPayload{ m_MainMenuMusicStringLiteral, true, true, .3f };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);

		playSoundEvent.Deliver();

        UnbindAllInput();

        BindInput(GLFW_KEY_ENTER, KEY_PRESS, &CustomGame::StartGame);
        BindInput(GLFW_KEY_TAB, KEY_PRESS, &CustomGame::OpenInstructions);
        BindInput(GLFW_KEY_ESCAPE, KEY_PRESS, &CustomGame::ExitGame);
    }

    else if (newState == GameState::GAME_INSTRUCTIONS)
    {
        UnbindAllInput();

        BindInput(GLFW_KEY_TAB, KEY_PRESS, &CustomGame::CloseInstructions);
    }

    else if (newState == GameState::GAME_RUNNING)
    {
		AudioManager::PlaySoundEvent playSoundPayload{ m_GameMusicStringLiteral, true, true, .5f };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEvent));

        UnbindAllInput();
    }

    else if (newState == GameState::GAME_OVER)
    {
        UnbindAllInput();

        BindInput(GLFW_KEY_ENTER, KEY_PRESS, &CustomGame::ResetGame);
        BindInput(GLFW_KEY_ESCAPE, KEY_PRESS, &CustomGame::OpenMenu);
    }
}

void CustomGame::ResetGame()
{
    if (m_currentState == GameState::GAME_OVER)
    {
        CleanGame();
        LoadGame("World.json");
        ChangeState(GameState::GAME_RUNNING);
    }
}

void CustomGame::LoadGame(const std::string& worldJSONName)
{
    LoadWorldFromJSON(worldJSONName);
}

void CustomGame::CleanGame()
{
    Datum& children = m_rootEntity.Children();
    for (size_t i = 0; i < children.Size(); ++i)
    {
        Scope& child = children.Get<Scope>(i);
        delete& child;
    }
    m_rootEntity.Children().Clear();

    ResetPhysicsWorld();
    m_worldState.Clear();
    m_gameClock.Reset();
}

void CustomGame::StartGame()
{
    if (m_currentState == GameState::GAME_MENU)
    {
        LevelGenerator::LoadLevel("DefaultLevel.txt");
        LoadGame("World.json");
        ChangeState(GameState::GAME_RUNNING);
    }
}

void CustomGame::ExitGame()
{
    glfwSetWindowShouldClose(m_window, true);
}

void CustomGame::OpenMenu()
{
    CleanGame();
    ChangeState(GameState::GAME_MENU);
}

void CustomGame::OpenInstructions()
{
    ChangeState(GameState::GAME_INSTRUCTIONS);
}

void CustomGame::CloseInstructions()
{
    ChangeState(GameState::GAME_MENU);
}

void CustomGame::ResetPhysicsWorld()
{
    if (m_physicsWorld != nullptr)
    {
        delete m_physicsWorld;
    }

    m_physicsWorld = new b2World(b2Vec2(0, m_worldGravity));
    m_physicsWorld->SetContactListener(&m_contactListener);
}
#pragma endregion

#pragma region Update Functions
void CustomGame::UpdateMenu()
{
    glm::vec4 screenScale(160, 90, 0, 0);
    RenderUtil::Render(nullptr, m_MenuTexture, glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), screenScale);
}

void CustomGame::UpdateGame()
{
    static const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);
    static float timeStep = 1.0f / 60.f;
    static int32 velocityIterations = 6;
    static int32 positionIterations = 2;

    static float timeUnconsumed = 0.0f;


    glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
    m_gameClock.UpdateGameTime(m_worldState.GetGameTime());

    float deltaTime = m_worldState.GetDeltaTime();
    for (timeUnconsumed += deltaTime; timeUnconsumed >= timeStep; timeUnconsumed -= timeStep)
    {
        m_physicsWorld->Step(timeStep, velocityIterations, positionIterations);
    }

    m_totalGameTime += deltaTime;
    m_rootEntity.Update(&m_worldState);
    m_worldState.CompletePendingRequests();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    m_rootEntity.Render(&m_worldState);
}

void CustomGame::UpdateDeathScreen()
{
    static const glm::vec4 CornflowerBlue = glm::vec4(0.392f, 0.584f, 0.929f, 1.0f);

    glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_rootEntity.Render(&m_worldState);

    glm::vec4 screenScale(160, 90, 0, 0);
    RenderUtil::Render(nullptr, m_GameOverTexture, glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), screenScale);
}

void CustomGame::UpdateInstructions()
{
    glm::vec4 screenScale(160, 90, 0, 0);
    RenderUtil::Render(nullptr, m_InstructionsTexture, glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), screenScale);
}
#pragma endregion

void CustomGame::Notify(const EventPublisher& publisher)
{
    const Event<KeyboardEvent>* keyboardEvent = publisher.As<Event<KeyboardEvent>>();
    if (keyboardEvent)
    {
        auto it = m_InputActions.Find(keyboardEvent->Message());
        if (it != m_InputActions.end())
        {
            CustomGame::InputFunction func = it->second;
            (this->*func)();
        }
    }
}

void CustomGame::BindInput(int key, int action, InputFunction func)
{
    KeyboardEvent event{ key, action };
    m_InputActions[event] = func;
}

void CustomGame::UnbindInput(int key, int action)
{
    KeyboardEvent event{ key, action };
    m_InputActions.Remove(event);
}

void CustomGame::UnbindAllInput()
{
    m_InputActions.Clear();
}

CustomGame::CustomGame()
{
    Event<KeyboardEvent>::Subscribe(*this);
    ResetPhysicsWorld();
    OpenMenu();
    
}

CustomGame::~CustomGame()
{
    delete m_physicsWorld;
    Event<KeyboardEvent>::Unsubscribe(*this);
}
