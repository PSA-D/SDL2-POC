#pragma once

#include <cstddef>
#include <vector>

namespace game
{
// Geluiden worden in code gemaakt als 16-bit mono WAV (44,1 kHz) in het geheugen. Geen assets nodig.

// Kort piepje voor het gewone geluidseffect.
std::vector<std::byte> generateBeep();

// Drie loopende lagen (laag, midden, hoog). Ze slaan op dezelfde tel aan, zodat je hoort of ze gelijk lopen.
std::vector<std::vector<std::byte>> generateLayers();
} // namespace game
