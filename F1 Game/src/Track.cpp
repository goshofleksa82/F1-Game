#include "Track.h"

Track::Track()
    : m_name("Empty Track")
{
}

Track::Track(const std::string& name, const std::vector<Point>& checkpoints)
    : m_name(name), m_checkpoints(checkpoints)
{
}

const std::string& Track::GetName() const
{
    return m_name;
}

const std::vector<Point>& Track::GetCheckpoints() const
{
    return m_checkpoints;
}