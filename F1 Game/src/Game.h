#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Race.h"
#include "Track.h"

enum class GameState
{
    Menu,
    Racing,
    Result
};

class Game
{
public:
    Game();

    bool Init();
    void Run();
    void Shutdown();

private:
    void HandleEvents();
    void Update(float dt);
    void Render();

    void CreateTracks();
    void StartRace();

    void NextTrack();
    void NextLaps();
    void NextDifficulty();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_running;

    GameState m_state;

    std::vector<Track> m_tracks;
    int m_selectedTrack;
    int m_selectedLaps;
    Difficulty m_selectedDifficulty;

    Race m_race;
};