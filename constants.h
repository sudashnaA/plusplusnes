#pragma once
#include <cstdint>

inline constexpr uint8_t STKP_INITIAL_VALUE{ 0xFD };
inline constexpr uint16_t ADDR_ABS_INITIAL_VALUE{ 0xFFFC };
inline constexpr double PI{ 3.14159 };
inline constexpr unsigned int SAMPLE_RATE{ 44100 };
inline constexpr double PPU_CLOCK_FREQ{ 5369318.0 };

enum BitShift {
    zero_bits,
    one_bit,
    two_bits,
    three_bits,
    four_bits,
    five_bits,
    six_bits,
    seven_bits,
    eight_bits,
    nine_bits,
    ten_bits,
    eleven_bits,
    twelve_bits,
    thirteen_bits,
    fourteen_bits,
    fifteen_bits,
    sixteen_bits,
    max_bits,
};

// APU constants
enum APU_SEQUENCE_MODES {
    SEQUENCE_ONE = 3729,
    SEQUENCE_TWO = 7457,
    SEQUENCE_THREE = 11186,
    SEQUENCE_FOUR = 14916,
};

// Enum for mirroring modes
enum class MIRROR
{
    HARDWARE,
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
};

// PPU constants:
inline constexpr uint16_t COARSE_X_LIMIT{ 31 };