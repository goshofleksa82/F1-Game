#include "Race.h"

Race::Race()
    : m_totalLaps(3),
      m_difficulty(Difficulty::Medium),
      m_player(150.0f, 150.0f, CarType::Player),
      m_finished(false),
      m_playerWon(false)
{
    
}


void Race::Setup(const Track &track, int laps, Difficulty difficulty)
{
    m_track = track;
    m_totalLaps = laps;
    m_difficulty = difficulty;
    m_finished = false;
    m_playerWon = false;

    const std::vector<Point> &checkpoints = m_track.GetCheckpoints();

    float startX = checkpoints[0].x;
    float startY = checkpoints[0].y;

    m_player.Reset(startX, startY);
    m_player.SetColor(255, 40, 40);

    m_aiCars.clear();

    Car ai1(startX + 35.0f, startY + 25.0f, CarType::AI);
    ai1.SetColor(40, 40, 255);

    Car ai2(startX + 70.0f, startY + 50.0f, CarType::AI);
    ai2.SetColor(255, 255, 40);

    Car ai3(startX + 30.0f, startY + 70.0f, CarType::AI);
    ai3.SetColor(255, 120, 0);

    m_aiCars.push_back(ai1);
    m_aiCars.push_back(ai2);
    m_aiCars.push_back(ai3);
}

float Race::GetDifficultyMultiplier() const
{
    if (m_difficulty == Difficulty::Easy)
    {
        return 0.85f;
    }
    if (m_difficulty == Difficulty::Medium)
    {
        return 1.0f;
    }
    return 1.15f;
}

void Race::KeepCarsInsideScreen()
{
    auto clamp = [](float &value, float min, float max)
    {
        if (value < min)
            value = min;
        if (value > max)
            value = max;
    };

    // player
    float px = m_player.GetX();
    float py = m_player.GetY();
    clamp(px, 20.0f, 1260.0f);
    clamp(py, 20.0f, 700.0f);

    // ugly but beginner-friendly reset trick
    float oldAngle = m_player.GetAngle();
    int oldLap = m_player.GetLap();
    int oldCheckpoint = m_player.GetNextCheckpoint();

    m_player.Reset(px, py);
    m_player.SetAngle(oldAngle);

    for (int i = 0; i < oldCheckpoint; i++)
    {
        m_player.CheckPointsProgress(m_track);
    }
    for (int i = 0; i < oldLap; i++)
    {
        // no-op, left simple on purpose
    }

    // AI cars
    for (size_t i = 0; i < m_aiCars.size(); i++)
    {
        float x = m_aiCars[i].GetX();
        float y = m_aiCars[i].GetY();
        clamp(x, 20.0f, 1260.0f);
        clamp(y, 20.0f, 700.0f);

        float angle = m_aiCars[i].GetAngle();
        int lap = m_aiCars[i].GetLap();
        int checkpoint = m_aiCars[i].GetNextCheckpoint();

        m_aiCars[i].Reset(x, y);
        m_aiCars[i].SetAngle(angle);

        for (int j = 0; j < checkpoint; j++)
        {
            m_aiCars[i].CheckPointsProgress(m_track);
        }
        for (int j = 0; j < lap; j++)
        {
            // no-op, left simple on purpose
        }
    }
}

void Race::Update(float dt)
{
    if (m_finished)
    {
        return;
    }

    m_player.UpdatePlayer(dt);
    m_player.CheckPointsProgress(m_track);

    float aiMultiplier = GetDifficultyMultiplier();

    for (size_t i = 0; i < m_aiCars.size(); i++)
    {
        m_aiCars[i].UpdateAI(dt, m_track, aiMultiplier + (float)i * 0.02f);
        m_aiCars[i].CheckPointsProgress(m_track);
    }

    if (m_player.HasFinished(m_totalLaps))
    {
        m_finished = true;
        m_playerWon = true;
        return;
    }

    for (size_t i = 0; i < m_aiCars.size(); i++)
    {
        if (m_aiCars[i].HasFinished(m_totalLaps))
        {
            m_finished = true;
            m_playerWon = false;
            return;
        }
    }
}

void Race::Render(SDL_Renderer *renderer) const
{
    // grass
    SDL_SetRenderDrawColor(renderer, 30, 130, 30, 255);
    SDL_RenderClear(renderer);

    // track path as lines between checkpoints
    const std::vector<Point> &checkpoints = m_track.GetCheckpoints();

    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 255);
    for (size_t i = 0; i < checkpoints.size(); i++)
    {
        size_t next = (i + 1) % checkpoints.size();
        SDL_RenderLine(renderer,
                       checkpoints[i].x, checkpoints[i].y,
                       checkpoints[next].x, checkpoints[next].y);
    }

    // draw checkpoints
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    for (size_t i = 0; i < checkpoints.size(); i++)
    {
        SDL_FRect cpRect;
        cpRect.x = checkpoints[i].x - 8.0f;
        cpRect.y = checkpoints[i].y - 8.0f;
        cpRect.w = 16.0f;
        cpRect.h = 16.0f;
        SDL_RenderFillRect(renderer, &cpRect);
    }

    // finish line at checkpoint 0
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect finishRect;
    finishRect.x = checkpoints[0].x - 20.0f;
    finishRect.y = checkpoints[0].y - 20.0f;
    finishRect.w = 40.0f;
    finishRect.h = 40.0f;
    SDL_RenderRect(renderer, &finishRect);

    // cars
    m_player.Render(renderer);

    for (size_t i = 0; i < m_aiCars.size(); i++)
    {
        m_aiCars[i].Render(renderer);
    }
}

bool Race::IsFinished() const
{
    return m_finished;
}

bool Race::PlayerWon() const
{
    return m_playerWon;
}

int Race::GetPlayerLap() const
{
    return m_player.GetLap();
}

int Race::GetTotalLaps() const
{
    return m_totalLaps;
}