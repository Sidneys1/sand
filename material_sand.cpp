#include "include/constants.h"
#include "include/kernels.h"
#include "include/sandbox.h"
#include "include/util.h"

#include <cstdlib>
#include <limits>

template <> void Sandbox::InitMaterialType<SAND>(::Mixel &mixel) {
    mixel.sand.color = m_dist_sand_color(m_rng) - 15;
    mixel.sand.wetness = 0;
}

template <> void Sandbox::HandleMaterialType<SAND>(int x, int y, ::Mixel *const (&kernel)[3]) {
    ::Mixel &self = kernel[KERNEL_MIDDLE][KERNEL_CENTER];

    ::Mixel &below = kernel[KERNEL_BOTTOM][KERNEL_CENTER];

    MaterialType below_type = below.type;
    MaterialType left_type = kernel[KERNEL_MIDDLE][KERNEL_LEFT].type;
    MaterialType right_type = kernel[KERNEL_MIDDLE][KERNEL_RIGHT].type;
    MaterialType dl_type = kernel[KERNEL_BOTTOM][KERNEL_LEFT].type;
    MaterialType dr_type = kernel[KERNEL_BOTTOM][KERNEL_RIGHT].type;

    if (self.sand.wetness >= 2 && m_dist_spread(m_rng) <= 2) [[unlikely]] {
        if (below_type == SAND && below.sand.wetness <= MAX_WETNESS && self.sand.wetness - below.sand.wetness > -1) {
            --self.sand.wetness;
            ++below.sand.wetness;
        }

        if (left_type == SAND && self.sand.wetness - kernel[KERNEL_MIDDLE][KERNEL_LEFT].sand.wetness > 1) {
            --self.sand.wetness;
            ++kernel[KERNEL_MIDDLE][KERNEL_LEFT].sand.wetness;
        }

        if (right_type == SAND && self.sand.wetness - kernel[KERNEL_MIDDLE][KERNEL_RIGHT].sand.wetness > 1) {
            --self.sand.wetness;
            ++kernel[KERNEL_MIDDLE][KERNEL_RIGHT].sand.wetness;
        }

        if (dl_type == SAND && self.sand.wetness - kernel[KERNEL_BOTTOM][KERNEL_LEFT].sand.wetness > 1) {
            --self.sand.wetness;
            ++kernel[KERNEL_BOTTOM][KERNEL_LEFT].sand.wetness;
        }

        if (dr_type == SAND && self.sand.wetness - kernel[KERNEL_BOTTOM][KERNEL_RIGHT].sand.wetness > 1) {
            --self.sand.wetness;
            ++kernel[KERNEL_BOTTOM][KERNEL_RIGHT].sand.wetness;
        }
    }

    bool solid_below = y == HEIGHT - 1 || (below_type != BLANK && below_type != WATER);

    bool solid_dleft =
        y == HEIGHT - 1 || x == 0 ||
        (kernel[KERNEL_BOTTOM][KERNEL_LEFT].type != BLANK && kernel[KERNEL_BOTTOM][KERNEL_LEFT].type != WATER);
    bool solid_dright =
        y == HEIGHT - 1 || x == WIDTH - 1 ||
        (kernel[KERNEL_BOTTOM][KERNEL_RIGHT].type != BLANK && kernel[KERNEL_BOTTOM][KERNEL_RIGHT].type != WATER);

    if (!solid_below && !solid_dleft && !solid_dright) {
        if (m_dist_spread(m_rng) == 0) {
            auto swap_with = m_dist(m_rng) ? TO_INDEX(x - 1, y + 1) : TO_INDEX(x + 1, y + 1);
            std::swap(Field[TO_INDEX(x, y)], Field[swap_with]);
            return;
        }
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x, y + 1)]);
        return;
    } else if (!solid_below) {
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x, y + 1)]);
        return;
    }

    bool solid_right =
        x == WIDTH - 1 || (Field[TO_INDEX(x + 1, y)].type != BLANK && Field[TO_INDEX(x + 1, y)].type != WATER);
    bool solid_left = x == 0 || (Field[TO_INDEX(x - 1, y)].type != BLANK && Field[TO_INDEX(x - 1, y)].type != WATER);

    if (!solid_dleft) {
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x - 1, y + 1)]);
        return;
    }
    if (!solid_dright) {
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x + 1, y + 1)]);
        return;
    }

    if (self.sand.wetness <= 3)
        return;

    bool solid_dlleft = y == HEIGHT - 1 || x <= 1 ||
                        (Field[TO_INDEX(x - 2, y + 1)].type != BLANK && Field[TO_INDEX(x - 2, y + 1)].type != WATER);
    bool solid_drright = y == HEIGHT - 1 || x >= WIDTH - 2 ||
                         (Field[TO_INDEX(x + 2, y + 1)].type != BLANK && Field[TO_INDEX(x + 2, y + 1)].type != WATER);

    if (!(solid_left || solid_dlleft)) {
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x - 1, y)]);
        return;
    }
    if (!(solid_right || solid_drright)) {
        std::swap(Field[TO_INDEX(x, y)], Field[TO_INDEX(x + 1, y)]);
        return;
    }
}
