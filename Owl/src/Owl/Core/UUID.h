#pragma once

#include <xhash>

namespace Owl
{
    class Uuid
    {
    public:
        Uuid();
        Uuid(uint64_t pUuid);
        Uuid(const Uuid&) = default;
        
        operator uint64_t() const { return m_Uuid; }
    private:
        uint64_t m_Uuid;
    };
}

template<>
struct std::hash<Owl::Uuid>
{
    std::size_t operator()(const Owl::Uuid& pUuid) const noexcept
    {
        return hash<uint64_t>()(pUuid);
    }
};
