
/*
 *
 *  We use the cpp file for conditions that need to be cached for a reason
 *
 *
 */

#if defined(NEKO_TF2)

#include "../../../framework/game.hpp"  // Stuff to init with
#include "../../../util/chrono.hpp"     // For time keeping
#include "../hacks/ifaces.hpp"

#include "conditions.hpp"

namespace modules::source {
namespace conditions {

// Time when the local player started zooming
static CatTimer zoom_timer;

static void WorldTick() {
    auto local_ent =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer());
    if (!HasCondition((CatEntity*)local_ent, TfCond::kZoomed))
        zoom_timer.Reset();
}

void Init() { events::world.Listen(WorldTick, cat::Order::kBefore); }

}  // namespace conditions

bool CanHeadshot() {
    // Get local ent
    auto local_ent =
        iface::entity_list->GetClientEntity(iface::engine->GetLocalPlayer());
    if (!local_ent || local_ent->IsDormant()) return false;
    // Check if currently zoomed
    if (!HasCondition((CatEntity*)local_ent, TfCond::kZoomed)) return false;
    // Check if zoomed long enough
    return conditions::zoom_timer.CheckTime(std::chrono::milliseconds(200));
}

}  // namespace modules::source

#endif
