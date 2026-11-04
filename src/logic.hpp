#pragma once
#include <cstdint>
namespace qol {
inline bool release_tick(std::uint32_t tick,int interval){
 return interval>=2 && (tick%static_cast<std::uint32_t>(interval))==static_cast<std::uint32_t>(interval-1);
}
inline bool shoulder_chord(std::uint8_t second_button_byte){
 return (second_button_byte & 0x0fu) != 0x0fu;
}
}
