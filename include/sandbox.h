#pragma once

#include <random>
#include <type_traits>

#include "./constants.h"
#include "./olcPixelGameEngine.h"
#include "./tools.h"

using uniform_dist = std::uniform_int_distribution<std::mt19937::result_type>;

enum class Direction { Up, Down, Left, Right };

class Sandbox : public olc::PixelGameEngine {
  public:
    enum class RenderMode { Normal, Temperature, /* DEBUG, */ END_OF_LIST };

  public:
    Sandbox();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;

  private:
    void ProcessPixel(int x, int y);
    template <MaterialType t> void InitMaterialType(::Mixel &);
    template <MaterialType t> void HandleMaterialType(int x, int y, ::Mixel *const (&)[3]) {}
    template <MaterialType t> void FillKernel(int size, int x, int y);

  private:
    union {
        char c;
        MaterialType t;
    } m_tool;
    char m_size = 9;
    char m_tool_temp = 0;
    RenderMode m_render_mode = RenderMode::Normal;

    std::random_device m_dev;
    std::mt19937 m_rng;
    uniform_dist m_dist;
    uniform_dist m_dist_spread;
    uniform_dist m_dist_sand_color;
    uniform_dist m_dist_10000;
    static Mixel Field[WIDTH * HEIGHT];
};

// Specializations

template <> void Sandbox::InitMaterialType<SAND>(::Mixel &);
template <> void Sandbox::HandleMaterialType<SAND>(int x, int y, ::Mixel *const (&)[3]);

template <> void Sandbox::InitMaterialType<WATER>(::Mixel &);
template <> void Sandbox::HandleMaterialType<WATER>(int x, int y, ::Mixel *const (&)[3]);

template <> void Sandbox::InitMaterialType<IRON>(::Mixel &);
template <> void Sandbox::HandleMaterialType<IRON>(int x, int y, ::Mixel *const (&)[3]);
