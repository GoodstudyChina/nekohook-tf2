
/*
 *
 *	Source engines world to screen framework interface
 *
 *
 */

#include "../../../framework/drawing.hpp"  // So we can put our world to screen into the framework
#include "../../../framework/game.hpp"   // So we can reset the wts cache
#include "../../../framework/input.hpp"  // For window bounds

#include "../hacks/ifaces.hpp"  // For g_IEngine + sdk
#include "../util/mathlib.hpp"  // To convert between vector types

#include "worldtoscreen.hpp"

namespace modules::source::wts {

// externed so overridehook can set it
CViewSetup* wts_viewsetup;
// Cached wts
static VMatrix wts;

// Our world to screen function
static bool WorldToScreen(CatVector world, std::pair<int, int>& screen) {
    // Sick P@sted math from original cathook
    float w = wts[3][0] * world.x + wts[3][1] * world.y + wts[3][2] * world.z +
              wts[3][3];
    float odw = 1.0f / w;
    auto bounds = input::GetBounds();
    screen.first =
        (bounds.first / 2) + (0.5 *
                                  ((wts[0][0] * world.x + wts[0][1] * world.y +
                                    wts[0][2] * world.z + wts[0][3]) *
                                   odw) *
                                  bounds.first +
                              0.5);
    screen.second =
        (bounds.second / 2) - (0.5 *
                                   ((wts[1][0] * world.x + wts[1][1] * world.y +
                                     wts[1][2] * world.z + wts[1][3]) *
                                    odw) *
                                   bounds.second +
                               0.5);

    return w > 0.001;
}

// We run this to make the framework use our world to screen
void Init() {
    draw::WorldToScreen = WorldToScreen;
    events::draw.Listen(
        []() {
            if (!wts_viewsetup) return;
            iface::engine_tool->GetWorldToScreenMatrixForView(*wts_viewsetup,
                                                              &wts);
        },
        cat::Order::kBefore);
}

}  // namespace modules::source::wts
