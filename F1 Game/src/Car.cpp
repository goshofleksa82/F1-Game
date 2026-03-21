#include "Car.h"
#include <cmath>

Car::Car()
    : m_x(0.0f), m_y(0.0f), m_speed(0.0f), m_angle(0.0f),
      m_maxSpeed(260.0f), m_acceleration(180.0f), m_turnSpeed(2.8f),
      m_nextCheckpoint(0), m_lap(0),
      m_colorR(255), m_colorG(255), m_colorB(255),
      m_type(CarType::Player)
{
}

Car::Car(float x, float y, CarType type)
    : m_x(x), m_y(y), m_speed(0.0f), m_angle(0.0f),
      m_maxSpeed(260.0f), m_acceleration(180.0f), m_turnSpeed(2.8f),
      m_nextCheckpoint(0), m_lap(0),
      m_colorR(255), m_colorG(255), m_colorB(255),
      m_type(type)
{
}

void Car::Reset(float x, float y)
{
    m_x = x;
    m_y = y;
    m_speed = 0.0f;
    m_angle = 0.0f;
    m_nextCheckpoint = 0;
    m_lap = 0;
}

void Car::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
}

void Car::SetAngle(float angle)
{
    m_angle = angle;
}

float Car::GetAngle() const
{
    return m_angle;
}

float Car::GetX() const
{
    return m_x;
}

float Car::GetY() const
{
    return m_y;
}

int Car::GetLap() const
{
    return m_lap;
}

int Car::GetNextCheckpoint() const
{
    return m_nextCheckpoint;
}

bool Car::HasFinished(int targetLaps) const
{
    return m_lap >= targetLaps;
}

float Car::Distance(float x1, float y1, float x2, float y2) const
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void Car::CheckPointsProgress(const Track &track)
{
    const std::vector<Point> &checkpoints = track.GetCheckpoints();
    if (checkpoints.empty())
    {
        return;
    }

    Point target = checkpoints[m_nextCheckpoint];
    float dist = Distance(m_x, m_y, target.x, target.y);

    if(dist < 35.0f) {
        m_nextCheckpoint++;

        if(m_nextCheckpoint >= (int)checkpoints.size()) {
            m_nextCheckpoint = 0;
            m_lap++;
        }
    }
}

void Car::UpdatePlayer(float dt)
{
    const bool* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
        m_speed += m_acceleration * dt;
    }

    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
        m_speed -= m_acceleration * dt;
    }

    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
        m_angle -= m_turnSpeed * dt;
    }

    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
        m_angle += m_turnSpeed * dt;
    }

    if (m_speed > m_maxSpeed) {
        m_speed = m_maxSpeed;
    }

    if (m_speed < -100.0f) {
        m_speed = -100.0f;
    }

    m_speed *= 0.985f;

    m_x += std::cos(m_angle) * m_speed * dt;
    m_y += std::sin(m_angle) * m_speed * dt;
}

void Car::UpdateAI(float dt, const Track& track, float aiSpeedMultiplier)
{
    const std::vector<Point>& checkpoints = track.GetCheckpoints();
    if (checkpoints.empty()) {
        return;
    }

    Point target = checkpoints[m_nextCheckpoint];

    float dx = target.x - m_x;
    float dy = target.y - m_y;
    float desiredAngle = std::atan2(dy, dx);

    m_angle = desiredAngle;
    m_speed = 170.0f * aiSpeedMultiplier;

    m_x += std::cos(m_angle) * m_speed * dt;
    m_y += std::sin(m_angle) * m_speed * dt;
}

void Car::Render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, m_colorR, m_colorG, m_colorB, 255);

    SDL_FRect rect;
    rect.w = 22.0f;
    rect.h = 12.0f;
    rect.x = m_x - rect.w / 2.0f;
    rect.y = m_y - rect.h / 2.0f;

    SDL_RenderFillRect(renderer, &rect);
}
