#pragma once

#include <array>

#define KERNEL_TOP 0
#define KERNEL_MIDDLE 1
#define KERNEL_BOTTOM 2
#define KERNEL_LEFT 0
#define KERNEL_CENTER 1
#define KERNEL_RIGHT 2

static constexpr std::array<std::array<bool, 3>, 3> x3_KERNEL = {
    {{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}};

static constexpr std::array<std::array<bool, 5>, 5> x5_KERNEL = {
    {{0, 1, 1, 1, 0},
     {1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1},
     {0, 1, 1, 1, 0}}};

static constexpr std::array<std::array<bool, 7>, 7> x7_KERNEL = {
    {{0, 0, 1, 1, 1, 0, 0},
     {0, 1, 1, 1, 1, 1, 0},
     {1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1},
     {0, 1, 1, 1, 1, 1, 0},
     {0, 0, 1, 1, 1, 0, 0}}};

static constexpr std::array<std::array<bool, 9>, 9> x9_KERNEL = {
    {{0, 0, 1, 1, 1, 1, 1, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 0},
     {1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1},
     {0, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 1, 1, 1, 1, 1, 0, 0}}};

static constexpr std::array<std::array<bool, 11>, 11> x11_KERNEL = {
    {{0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
     {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
     {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},}};

