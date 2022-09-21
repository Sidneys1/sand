#include "include/constants.h"
#include "include/kernels.h"
#include "include/sandbox.h"
#include "include/tools.h"
#include "include/util.h"

#include <limits>

Mixel Sandbox::Field[WIDTH * HEIGHT] = {};

Sandbox::Sandbox()
    : m_rng(m_dev()), m_dist(0, 1), m_dist_spread(0, 30), m_dist_sand_color(0, 30), m_dist_10000(0, 9999) {
    // Name your application
    std::ostringstream ss;
    ss << "PixelSandEngine (" << FormatWithCommas(WIDTH * HEIGHT) << " pixels)";
    sAppName = ss.str();

    m_tool.t = SAND;

    std::cout << "Field is " << sizeof(Field) / 1024.0f << "KiB (" << FormatWithCommas(WIDTH * HEIGHT) << " x "
              << FormatWithCommas(sizeof(Mixel)) << "B)" << std::endl;
}

bool Sandbox::OnUserCreate() {
    Clear(olc::BLACK);
    return true;
}

void Interact(Direction d, ::Mixel &a, ::Mixel &b) {
    if (std::abs(a.temp - b.temp) > 1) {
        if (a.temp > b.temp) {
            --a.temp;
            ++b.temp;
        } else {
            ++a.temp;
            --b.temp;
        }
    }
}

[[gnu::flatten]] void Sandbox::ProcessPixel(int x, int y) {
    const int ROW_OFF = WIDTH * y;

    ::Mixel *const kernel_top = &Field[TO_INDEX(x - 1, y - 1)];
    ::Mixel *const kernel_middle = &Field[TO_INDEX(x - 1, y)];
    ::Mixel *const kernel_bottom = &Field[TO_INDEX(x - 1, y + 1)];

    ::Mixel *const kernel[3]{kernel_top, kernel_middle, kernel_bottom};

    ::Mixel &self = Field[ROW_OFF + x];

    if (self.type != BLANK) [[unlikely]] {
        if (y != 0 && kernel[KERNEL_TOP][KERNEL_CENTER].type != BLANK)
            Interact(Direction::Up, self, kernel[KERNEL_TOP][KERNEL_CENTER]);
        if (x != WIDTH - 1 && kernel[KERNEL_MIDDLE][KERNEL_RIGHT].type != BLANK)
            Interact(Direction::Right, self, kernel[KERNEL_MIDDLE][KERNEL_RIGHT]);
        if (y != HEIGHT - 1 && kernel[KERNEL_BOTTOM][KERNEL_CENTER].type != BLANK)
            Interact(Direction::Down, self, kernel[KERNEL_BOTTOM][KERNEL_CENTER]);
        if (x != 0 && kernel[KERNEL_MIDDLE][KERNEL_LEFT].type != BLANK)
            Interact(Direction::Left, self, kernel[KERNEL_MIDDLE][KERNEL_LEFT]);
    }

#define x(z)                                                                                                           \
    case z:                                                                                                            \
        HandleMaterialType<z>(x, y, kernel);                                                                           \
        break;
    switch (Field[ROW_OFF + x].type) { MATERIALS }
#undef x
}

template <MaterialType t> void Sandbox::InitMaterialType(::Mixel &mixel) {}

#define KERNEL_SIZES x(3) x(5) x(7) x(9) x(11)

template <MaterialType t> void Sandbox::FillKernel(int size, int x, int y) {
    auto pos_off_x = x;
    auto pos_off_y = y - 8;

    if (size == 1) [[unlikely]] {
        auto &target = Field[TO_INDEX(pos_off_x, pos_off_y)];
        target.type = m_tool.t;
        InitMaterialType<t>(target);
        return;
    }

    int adjust = size / 2;
    pos_off_x -= adjust;
    pos_off_y -= adjust;

    for (auto y = 0; y < size; y++) {
        for (auto x = 0; x < size; x++) {
            bool in_kernel = false;
#define x(z)                                                                                                           \
    case z:                                                                                                            \
        in_kernel = x##z##_KERNEL[y][x];                                                                               \
        break;
            switch (size) { KERNEL_SIZES }
#undef x

            if (in_kernel && IN_BOUNDS(pos_off_x + x, pos_off_y + y)) {
                auto &self = Field[TO_INDEX(pos_off_x + x, pos_off_y + y)];
                self.type = m_tool.t;
                self.temp = m_tool_temp;
                InitMaterialType<t>(self);
            }
        }
    }
}

[[gnu::flatten]] bool Sandbox::OnUserUpdate(float fElapsedTime) {
    static float time_since_last_update = 0;
    time_since_last_update += fElapsedTime;

    // Process Mouse
    if (GetMouse(olc::Mouse::LEFT).bHeld) {
        const auto &mouse_pos = GetMousePos();
#define x(z)                                                                                                           \
    case z:                                                                                                            \
        FillKernel<z>(m_size, mouse_pos.x, mouse_pos.y);                                                               \
        break;
        switch (m_tool.t) { MATERIALS }
#undef x
    } else if (GetMouse(olc::Mouse::RIGHT).bHeld) {
        const auto &mouse_pos = GetMousePos();
        FillKernel<BLANK>(m_size, mouse_pos.x, mouse_pos.y);
    }

    if (auto this_mouse_wheel = GetMouseWheel(); this_mouse_wheel) {
        if (GetKey(olc::Key::CTRL).bHeld) {
            m_size += this_mouse_wheel > 0 ? 2 : -2;
            if (m_size > 11)
                m_size = 11;
            else if (m_size < 1)
                m_size = 1;
        } else if (GetKey(olc::Key::T).bHeld) {
            m_tool_temp += (GetKey(olc::Key::SHIFT).bHeld ? 10 : 1) * (this_mouse_wheel > 0 ? 1 : -1);
        } else {
            if (this_mouse_wheel > 0)
                m_tool.c++;
            else
                m_tool.c--;
            m_tool.c %= NUM_TOOLS;
        }
    }

    FillRect(0, 0, WIDTH, 8, olc::BLACK);
    DrawString(0, 0, std::to_string(m_size));
    DrawString(20, 0, TOOL_NAMES[m_tool.c]);
    DrawString(75, 0, std::to_string(m_tool_temp) + " degrees");
    switch (m_render_mode) {
        case Sandbox::RenderMode::Normal: {
            DrawString(170, 0, "Normal");
        } break;
        case Sandbox::RenderMode::Temperature: {
            DrawString(170, 0, "Temperature");
        } break;
    }

    if (GetKey(olc::Key::R).bPressed) [[unlikely]] {
        ++m_render_mode;
    }

    if (GetKey(olc::Key::D).bPressed) [[unlikely]] {
        const auto &mouse_pos = GetMousePos();
#define x(z)                                                                                                           \
    case z:                                                                                                            \
        FillKernel<z>(1, mouse_pos.x, mouse_pos.y);                                                                    \
        break;
        switch (m_tool.t) { MATERIALS }
#undef x
    }

    if (!GetKey(olc::Key::SPACE).bHeld) [[likely]] {
        if (time_since_last_update < FRAME_TIME)
            return true;
        time_since_last_update -= FRAME_TIME;
    }

    // if (m_render_mode == RenderMode::DEBUG) {
    //     Clear(olc::BLACK);
    // }

    static int start = 0;
    for (int y = HEIGHT - 1; y >= 0; --y) {
        if (++start % 2) {
            for (int x = 0; x < WIDTH; x += 2) {
                ProcessPixel(x, y);
            }
            for (int x = 1; x < WIDTH; x += 2) {
                ProcessPixel(x, y);
            }
        } else {
            for (int x = WIDTH - 1; x >= 0; x -= 2) {
                ProcessPixel(x, y);
            }
            for (int x = WIDTH - 2; x >= 0; x -= 2) {
                ProcessPixel(x, y);
            }
        }
    }

    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = WIDTH - 1; x >= 0; x--) {
            const ::Mixel &pixel = Field[TO_INDEX(x, y)];
            if (/* m_render_mode != RenderMode::DEBUG &&  */ pixel.type == BLANK) {
                Draw(x, y + 8, olc::BLACK);
                continue;
            }
            switch (m_render_mode) {
                case RenderMode::Normal:
                    [[likely]] {
                        olc::Pixel color(std::get<0>(MATERIAL_COLORS[pixel.type]),
                                         std::get<1>(MATERIAL_COLORS[pixel.type]),
                                         std::get<2>(MATERIAL_COLORS[pixel.type]));
                        switch (pixel.type) {
                            case SAND: {
                                color.r += pixel.sand.color;
                                color.g += pixel.sand.color;
                                color.b += pixel.sand.color;

                                color.r -= pixel.sand.wetness * 9;
                                color.g -= pixel.sand.wetness * 9;
                                color.b -= pixel.sand.wetness * 9;
                            } break;
                            case WATER: {
                                if (pixel.temp < WATER_FREEZE) {
                                    color.r = color.g = 150;
                                    color.r = color.g += sin((x + y) / 4.0f) * 15;
                                } else
                                    color.r = color.g += (rand() % 30);
                            } break;
                            case IRON: {
                                if (pixel.iron.rusty) {
                                    color.g -= 50;
                                    color.b -= 50;
                                }
                            }
                            default:
                                break;
                        }
                        Draw(x, y + 8, color);
                    }
                    break;
                case RenderMode::Temperature: {
                    if (pixel.temp > 0) {
                        auto temp = pixel.temp / (float)std::numeric_limits<char>::max();
                        auto redness = std::numeric_limits<unsigned char>::max() * temp;
                        Draw(x, y + 8, olc::Pixel(255, 255 - redness, 255 - redness));
                    } else {
                        auto temp = pixel.temp / (float)std::numeric_limits<char>::min();
                        auto blueness = std::numeric_limits<unsigned char>::max() * temp;
                        Draw(x, y + 8, olc::Pixel(255 - blueness, 255 - blueness, 255));
                    }

                } break;
                    // case RenderMode::DEBUG: {

                    // } break;
            }
        }
    }

    return true;
}
