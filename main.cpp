#define OLC_PGE_APPLICATION
#include "include/olcPixelGameEngine.h"

#include "include/constants.h"
#include "include/sandbox.h"

int main() {
    Sandbox demo;
    if (demo.Construct(WIDTH, HEIGHT + 8, ZOOM, ZOOM))
        demo.Start();
    return 0;
}
