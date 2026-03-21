#pragma once
#include <vector>
#include "Car.h"
#include "Track.h"

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

class Race
{
public:
    Race();

    void Setup(const Track& track, int laps, Difficulty difficulty);
    void Update(float dt);
    void Render(SDL_Renderer* renderer) const;

    bool IsFinished() const;
    bool PlayerWon() const;

    int GetPlayerLap() const;
    int GetTotalLaps() const;

private:
    float GetDifficultyMultiplier() const;
    void KeepCarsInsideScreen();

private:
    Track m_track;
    int m_totalLaps;
    Difficulty m_difficulty;

    Car m_player;
    std::vector<Car> m_aiCars;

    bool m_finished;
    bool m_playerWon;
};