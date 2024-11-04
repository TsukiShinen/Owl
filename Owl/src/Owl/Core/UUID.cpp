#include "opch.h"
#include "UUID.h"

#include <random>

namespace Owl
{
    static std::random_device s_RandomDevice;
    static std::mt19937_64 s_Engine(s_RandomDevice());
    static std::uniform_int_distribution<uint64_t> s_UniformDistribution;
    
    Uuid::Uuid()
        : m_Uuid(s_UniformDistribution(s_Engine))
    {
    }

    Uuid::Uuid(const uint64_t pUuid)
        : m_Uuid(pUuid)
    {
    }
}
