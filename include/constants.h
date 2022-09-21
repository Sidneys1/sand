#pragma once

#include <cstdint>

static constexpr int32_t WIDTH = 250;
static constexpr int32_t HEIGHT = 250;
static constexpr int32_t ZOOM = 3;
static constexpr float TARGET_FPS = 60.0f;
static constexpr float FRAME_TIME = 1.0f / TARGET_FPS;

static constexpr auto MAX_WETNESS = 10;
static constexpr char WATER_FREEZE = -50;
