#pragma once
#include "Game.h"
#include "AudioManager.h"
#include <map>

using namespace FieaGameEngine;

enum class GameState
{
    GAME_MENU,
    GAME_RUNNING,
    GAME_OVER,
    GAME_INSTRUCTIONS
};


class CustomGame : public Game, public EventSubscriber
{
public:

    static void Create();
    static CustomGame* GetCustomGame();

    virtual void Notify(const EventPublisher& publisher) override;

    virtual void Run() override;

    void ChangeState(GameState newState);
    


    void SetMainMenuImage(const std::string& filename);
    void SetInstructionsImage(const std::string& filename);
    void SetGameOverImage(const std::string& filename);


protected:
    void UpdateMenu();
    void UpdateGame();
    void UpdateDeathScreen();
    void UpdateInstructions();


    void ResetGame();
    void LoadGame(const std::string& worldJSONName);
    void CleanGame();
    void StartGame();
    void ExitGame();

    void OpenMenu();

    void OpenInstructions();
    void CloseInstructions();

    void ResetPhysicsWorld();

    CustomGame();
    ~CustomGame();

private:
    inline static CustomGame* s_customGameInstance;
    inline static const float m_worldGravity = -110.0f;
    WorldContactListener m_contactListener;

    TextureInfo m_MenuTexture;
    TextureInfo m_InstructionsTexture;
    TextureInfo m_GameOverTexture;

    GameState m_currentState = GameState::GAME_MENU;

    using UpdateFunction = void(CustomGame::*)();
    inline static const std::map<GameState, UpdateFunction> m_UpdateFunctions =
    {
        {GameState::GAME_MENU, &CustomGame::UpdateMenu},
        {GameState::GAME_RUNNING, &CustomGame::UpdateGame},
        {GameState::GAME_OVER, &CustomGame::UpdateDeathScreen},
        {GameState::GAME_INSTRUCTIONS, &CustomGame::UpdateInstructions}
    };

    using InputFunction = void(CustomGame::*)();
    void BindInput(int key, int action, InputFunction func);
    void UnbindInput(int key, int action);
    void UnbindAllInput();

    HashMap<KeyboardEvent, InputFunction> m_InputActions;

	AudioManager audioManager;
	std::string m_MainMenuMusicStringLiteral = "8BitHipHopRapInstrumentalMonkeyIsland.mp3";
	std::string m_GameMusicStringLiteral = "WITHINTHERUINS-ATAXIAIIDJ8NTREMIX8BIT.mp3";
};

