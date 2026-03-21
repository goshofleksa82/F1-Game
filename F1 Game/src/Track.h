#pragma once
#include <string>
#include <vector>

struct Point
{
    float x;
    float y;
};

class Track
{
public:
    Track();
    Track(const std::string& name, const std::vector<Point>& checkpoints);

    const std::string& GetName() const;
    const std::vector<Point>& GetCheckpoints() const;

private:
    std::string m_name;
    std::vector<Point> m_checkpoints;
};