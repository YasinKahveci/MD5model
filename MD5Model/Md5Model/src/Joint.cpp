#include "Joint.hpp"

void Joint::ComputeQuatW()
{
    float t = 1.0f - (m_Orient.x * m_Orient.x) - (m_Orient.y * m_Orient.y) - (m_Orient.z * m_Orient.z);
    if (t < 0.0f)
    {
        m_Orient.w = 0.0f;
    }
    else
    {
        m_Orient.w = -sqrtf(t);
    }
}

