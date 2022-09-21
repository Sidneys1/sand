#include "include/constants.h"
#include "include/kernels.h"
#include "include/sandbox.h"
#include "include/util.h"

template <> void Sandbox::InitMaterialType<IRON>(::Mixel &mixel) { mixel.iron.rusty = false; }

template <> void Sandbox::HandleMaterialType<IRON>(int x, int y, ::Mixel *const (&kernel)[3]) {
    ::Mixel &self = kernel[KERNEL_MIDDLE][KERNEL_CENTER];
    if (self.iron.rusty && m_dist_10000(m_rng) == 0) {
        self.type = BLANK;
    }
}
