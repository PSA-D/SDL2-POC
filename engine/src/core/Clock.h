#pragma once

#include <algorithm>
#include <chrono>

namespace engine::core
{
class Clock
{
public:
    // Seconden sinds de vorige tick. Begrensd, zodat een hapering (bv. venster slepen) geen enorme sprong geeft.
    float tick()
    {
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> elapsed = now - last;
        last = now;
        return std::min(elapsed.count(), MAX_DT);
    }

private:
    static constexpr float MAX_DT = 0.1F;

    std::chrono::steady_clock::time_point last{std::chrono::steady_clock::now()};
};
} // namespace engine::core
