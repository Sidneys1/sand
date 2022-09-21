#include "include/constants.h"
#include "include/kernels.h"
#include "include/sandbox.h"
#include "include/util.h"

#include <limits>

template <> void Sandbox::InitMaterialType<WATER>(::Mixel &mixel) {
    mixel.sand.color = m_dist_sand_color(m_rng) - 15;
    mixel.sand.wetness = 0;
}

template <> void Sandbox::HandleMaterialType<WATER>(int x, int y, ::Mixel *const (&kernel)[3]) {
    ::Mixel &self = kernel[KERNEL_MIDDLE][KERNEL_CENTER];

    if (self.temp < WATER_FREEZE)
        return;

    ::Mixel &below = kernel[KERNEL_BOTTOM][KERNEL_CENTER];
    MaterialType below_type = y == HEIGHT - 1 ? BLANK : below.type;

    switch (below_type) {
        [[likely]] default : break;
        case SAND: {
            if (below.sand.wetness <= MAX_WETNESS / 2) {
                below.sand.wetness += MAX_WETNESS / 2;
                self.type = BLANK;
                return;
            }
        } break;
        case IRON: {
            if (!below.iron.rusty) {
                self.type = BLANK;
                below.iron.rusty = true;
                return;
            }
        } break;
    }

    ::Mixel &left = kernel[KERNEL_MIDDLE][KERNEL_LEFT];
    MaterialType left_type = x == 0 ? BLANK : left.type;
    switch (left_type) {
        [[likely]] default : break;
        case SAND: {
            if (left.sand.wetness <= MAX_WETNESS / 2) {
                left.sand.wetness += MAX_WETNESS / 2;
                self.type = BLANK;
                return;
            }
        } break;
        case IRON: {
            if (!left.iron.rusty) {
                self.type = BLANK;
                left.iron.rusty = true;
                return;
            }
        } break;
    }

    ::Mixel &right = kernel[KERNEL_MIDDLE][KERNEL_RIGHT];
    MaterialType right_type = x == WIDTH - 1 ? BLANK : right.type;

    switch (right_type) {
        [[likely]] default : break;
        case SAND: {
            if (right.sand.wetness <= MAX_WETNESS / 2) {
                right.sand.wetness += MAX_WETNESS / 2;
                self.type = BLANK;
                return;
            }
        } break;
        case IRON: {
            if (!right.iron.rusty) {
                self.type = BLANK;
                right.iron.rusty = true;
                return;
            }
        } break;
    }

    bool at_bottom = y == (HEIGHT - 1);
    bool at_left = x == 0;
    bool at_right = x == (WIDTH - 1);

    bool solid_below = at_bottom || below_type != BLANK;

    bool solid_left = at_left || left_type != BLANK;
    bool solid_right = at_right || right_type != BLANK;

    bool solid_dleft = at_bottom || at_left || kernel[KERNEL_BOTTOM][KERNEL_LEFT].type != BLANK;
    bool solid_dright = at_bottom || at_right || kernel[KERNEL_BOTTOM][KERNEL_RIGHT].type != BLANK;

    if (solid_below && solid_left && solid_right && solid_dleft && solid_dright)
        return;

    int swap_with;

    if (!solid_below && !solid_dleft && !solid_dright) {
        if (m_dist_spread(m_rng) <= 5) {
            swap_with = m_dist(m_rng) ? (TO_INDEX(x + 1, y + 1)) : (TO_INDEX(x - 1, y + 1));
        } else {
            swap_with = TO_INDEX(x, y + 1);
        }
    } else if (!solid_below) {
        swap_with = TO_INDEX(x, y + 1);
    } else if ((!solid_dleft) && (!solid_dright)) {
        swap_with = m_dist(m_rng) ? (TO_INDEX(x - 1, y + 1)) : (TO_INDEX(x + 1, y + 1));
    } else if (!solid_dright) {
        swap_with = (TO_INDEX(x + 1, y + 1));
    } else if (!solid_dleft) {
        swap_with = (TO_INDEX(x - 1, y + 1));
    } else if ((!solid_left) && (!solid_right)) {
        swap_with = m_dist(m_rng) ? (TO_INDEX(x - 1, y)) : (TO_INDEX(x + 1, y));
    } else if (!solid_right) {
        swap_with = (TO_INDEX(x + 1, y));
    } else if (!solid_left) {
        swap_with = (TO_INDEX(x - 1, y));
    } else {
        std::cout << "edge case, bitch" << std::endl;
        return;
    }

    std::swap(Field[TO_INDEX(x, y)], Field[swap_with]);
}
