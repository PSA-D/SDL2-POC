#pragma once

#include <cstdint>

namespace engine::core
{
// Een entity is alleen een id; alle data zit in componenten.
using Entity = std::uint32_t;
} // namespace engine::core
