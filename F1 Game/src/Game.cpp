#include "Game.h"
#include <iostream>

Game::Game()
    : m_window(nullptr),
      m_renderer(nullptr),
      m_running(true),
      m_state(GameState::Menu),
      m_selectedTrack(0),
      m_selectedLaps(3),
      m_selectedDifficulty(Difficulty::Easy)
{
}

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    m_window = SDL_CreateWindow("Beginner F1 SDL3 Game", 1280, 720, 0);
    if (!m_window) {
        std::cout << "Window creation failed: " << SDL_GetError() << "\n";
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, NULL);
    if (!m_renderer) {
        std::cout << "Renderer creation failed: " << SDL_GetError() << "\n";
        return false;
    }

    CreateTracks();

    std::cout << "=============================\n";
    std::cout << "F1 GAME\n";
    std::cout << "=============================\n";
    std::cout << "MENU CONTROLS:\n";
    std::cout << "T = change track\n";
    std::cout << "L = change laps\n";
    std::cout << "D = change difficulty\n";
    std::cout << "ENTER = start race\n";
    std::cout << "\nRACE CONTROLS:\n";
    std::cout << "Arrow keys or WASD to drive\n";
    std::cout << "ESC = back to menu\n";
    std::cout << "\n";

    return true;
}

void Game::CreateTracks()
{
    std::vector<Point> track1;
    track1.push_back({150, 150});
    track1.push_back({500, 100});
    track1.push_back({950, 120});
    track1.push_back({1100, 320});
    track1.push_back({1000, 580});
    track1.push_back({550, 620});
    track1.push_back({180, 500});
    track1.push_back({100, 300});

    std::vector<Point> track2;
    track2.push_back({200, 200});
    track2.push_back({400, 120});
    track2.push_back({700, 160});
    track2.push_back({980, 140});
    track2.push_back({1120, 300});
    track2.push_back({960, 480});
    track2.push_back({800, 620});
    track2.push_back({500, 560});
    track2.push_back({300, 640});
    track2.push_back({120, 450});
    track2.push_back({100, 280});

    m_tracks.push_back(Track("Oval Style", track1));
    m_tracks.push_back(Track("Technical Style", track2));
}

void Game::StartRace()
{
    m_race.Setup(m_tracks[m_selectedTrack], m_selectedLaps, m_selectedDifficulty);
    m_state = GameState::Racing;

    std::cout << "\nStarting race...\n";
    std::cout << "Track: " << m_tracks[m_selectedTrack].GetName() << "\n";
    std::cout << "Laps: " << m_selectedLaps << "\n";

    if (m_selectedDifficulty == Difficulty::Easy) {
        std::cout << "Difficulty: Easy\n";
    } else if (m_selectedDifficulty == Difficulty::Medium) {
        std::cout << "Difficulty: Medium\n";
    } else {
        std::cout << "Difficulty: Hard\n";
    }

    std::cout << "\n";
}

void Game::NextTrack()
{
    m_selectedTrack++;
    if (m_selectedTrack >= (int)m_tracks.size()) {
        m_selectedTrack = 0;
    }

    std::cout << "Selected track: " << m_tracks[m_selectedTrack].GetName() << "\n";
}

void Game::NextLaps()
{
    if (m_selectedLaps == 3) {
        m_selectedLaps = 5;
    } else if (m_selectedLaps == 5) {
        m_selectedLaps = 7;
    } else {
        m_selectedLaps = 3;
    }

    std::cout << "Selected laps: " << m_selectedLaps << "\n";
}

void Game::NextDifficulty()
{
    if (m_selectedDifficulty == Difficulty::Easy) {
        m_selectedDifficulty = Difficulty::Medium;
        std::cout << "Selected difficulty: Medium\n";
    } else if (m_selectedDifficulty == Difficulty::Medium) {
        m_selectedDifficulty = Difficulty::Hard;
        std::cout << "Selected difficulty: Hard\n";
    } else {
        m_selectedDifficulty = Difficulty::Easy;
        std::cout << "Selected difficulty: Easy\n";
    }
}

void Game::Run()
{
    Uint64 lastTicks = SDL_GetTicks();

    while (m_running) {
        Uint64 currentTicks = SDL_GetTicks();
        float dt = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        HandleEvents();
        Update(dt);
        Render();

        SDL_Delay(16);
    }
}

void Game::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Keycode key = event.key.key;

            if (m_state == GameState::Menu) {
                if (key == SDLK_T) {
                    NextTrack();
                } else if (key == SDLK_L) {
                    NextLaps();
                } else if (key == SDLK_D) {
                    NextDifficulty();
                } else if (key == SDLK_RETURN) {
                    StartRace();
                }
            }
            else if (m_state == GameState::Racing) {
                if (key == SDLK_ESCAPE) {
                    m_state = GameState::Menu;
                    std::cout << "Back to menu\n";
                }
            }
            else if (m_state == GameState::Result) {
                if (key == SDLK_RETURN) {
                    m_state = GameState::Menu;
                    std::cout << "Back to menu\n";
                }
            }
        }
    }
}

void Game::Update(float dt)
{
    if (m_state == GameState::Racing) {
        m_race.Update(dt);

        if (m_race.IsFinished()) {
            m_state = GameState::Result;

            if (m_race.PlayerWon()) {
                std::cout << "YOU WON!\n";
            } else {
                std::cout << "YOU LOST!\n";
            }

            std::cout << "Press ENTER to return to menu.\n";
        }
    }
}

void Game::Render()
{
    if (m_state == GameState::Menu) {
        SDL_SetRenderDrawColor(m_renderer, 20, 20, 20, 255);
        SDL_RenderClear(m_renderer);

        // simple menu panels
        SDL_SetRenderDrawColor(m_renderer, 60, 60, 60, 255);
        SDL_FRect panel1 = {100, 100, 1080, 100};
        SDL_FRect panel2 = {100, 240, 1080, 100};
        SDL_FRect panel3 = {100, 380, 1080, 100};
        SDL_FRect panel4 = {100, 520, 1080, 100};

        SDL_RenderFillRect(m_renderer, &panel1);
        SDL_RenderFillRect(m_renderer, &panel2);
        SDL_RenderFillRect(m_renderer, &panel3);
        SDL_RenderFillRect(m_renderer, &panel4);

        // colored indicators only
        SDL_SetRenderDrawColor(m_renderer, 200, 0, 0, 255);
        SDL_FRect a = {120, 120, 80, 60};
        SDL_RenderFillRect(m_renderer, &a);

        SDL_SetRenderDrawColor(m_renderer, 0, 180, 255, 255);
        SDL_FRect b = {120, 260, 80, 60};
        SDL_RenderFillRect(m_renderer, &b);

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255);
        SDL_FRect c = {120, 400, 80, 60};
        SDL_RenderFillRect(m_renderer, &c);

        SDL_SetRenderDrawColor(m_renderer, 0, 255, 100, 255);
        SDL_FRect d = {120, 540, 80, 60};
        SDL_RenderFillRect(m_renderer, &d);
    }
    else if (m_state == GameState::Racing) {
        m_race.Render(m_renderer);

        // very simple lap bar
        int lapWidth = 200;
        int currentLap = m_race.GetPlayerLap();
        int totalLaps = m_race.GetTotalLaps();

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_FRect back = {20, 20, (float)lapWidth, 24};
        SDL_RenderFillRect(m_renderer, &back);

        float ratio = 0.0f;
        if (totalLaps > 0) {
            ratio = (float)currentLap / (float)totalLaps;
        }

        if (ratio > 1.0f) {
            ratio = 1.0f;
        }

        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
        SDL_FRect front = {20, 20, lapWidth * ratio, 24};
        SDL_RenderFillRect(m_renderer, &front);
    }
    else if (m_state == GameState::Result) {
        if (m_race.PlayerWon()) {
            SDL_SetRenderDrawColor(m_renderer, 30, 120, 30, 255);
        } else {
            SDL_SetRenderDrawColor(m_renderer, 120, 30, 30, 255);
        }

        SDL_RenderClear(m_renderer);

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_FRect box = {340, 220, 600, 280};
        SDL_RenderRect(m_renderer, &box);
    }

    SDL_RenderPresent(m_renderer);
}

void Game::Shutdown()
{
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}