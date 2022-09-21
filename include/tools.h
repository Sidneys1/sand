#pragma once

#include "./forward.h"

#include <array>
#include <tuple>

#define MATERIALS x(BLANK) x(SAND) x(WATER) x(IRON)

#define TOOLS x(BLANK) x(SAND) x(WATER) x(IRON)

#define x(y) y,
enum MaterialType : char { MATERIALS LAST = IRON };
#undef x

constexpr std::size_t NUM_TOOLS = MaterialType::LAST + 1;
constexpr std::size_t NUM_MATERIALS = MaterialType::LAST + 1;

#define x(y) #y,
constexpr std::array<const char *, NUM_TOOLS> TOOL_NAMES = {TOOLS};
#undef x

using color_tuple = std::tuple<uint8_t, uint8_t, uint8_t>;

constexpr color_tuple SAND_COLOR = {194, 178, 128};
constexpr color_tuple IRON_COLOR = {128, 128, 128};
constexpr color_tuple WATER_COLOR = {0, 0, 255};
constexpr color_tuple BLANK_COLOR = {0, 0, 0};

#define x(y) y##_COLOR,
constexpr std::array<color_tuple, NUM_TOOLS> MATERIAL_COLORS = {TOOLS};
#undef x

typedef struct {
    MaterialType type;
    char temp;
    union {
        struct {
            char color;
            char wetness;
        } sand;
        struct {
            bool rusty;
        } iron;
    };
} Mixel;
