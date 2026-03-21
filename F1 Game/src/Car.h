#pragma once
#include <SDL3/SDL.h>
#include "Track.h"

enum class CarType
{
    Player, 
    AI
};

class Car{
    public:
    Car();
    Car(float x, float y, CarType type);

    void UpdatePlayer(float dt);
    void UpdateAI(float dt, const Track& track, float aiSpeedMultiplayer);
    void Render(SDL_Renderer* renderer) const;

    void Reset(float x, float y);

    float GetX() const;
    float GetY() const;
    int GetLap() const;
    int GetNextCheckpoint() const;
    bool HasFinished(int targetLaps) const;

    void SetColor(Uint8 r, Uint8 g, Uint8 b);
    void SetAngle(float angle);
    float GetAngle() const;

    void CheckPointsProgress(const Track& track);

    private:
    float Distance(float x1, float y1, float x2, float y2) const;

    private:
    float m_x;
    float m_y;
    float m_speed;
    float m_angle;
    float m_maxSpeed;
    float m_acceleration;
    float m_turnSpeed;

    int m_nextCheckpoint;
    int m_lap;

    Uint8 m_colorR;
    Uint8 m_colorG;
    Uint8 m_colorB;

    CarType m_type;
};