
/*
 *
 * Misselanious
 *
 */

#include "../../../framework/game.hpp"
#include "../../../util/catvars.hpp"
#include "../hacks/hooks.hpp"
#include "../hacks/ifaces.hpp"
#include "../util/classes.hpp"
#include "../util/netvars.hpp"
#if defined(NEKO_TF2)
#include "../util/conditions.hpp"
#endif

namespace modules::source::features::misc {

static CatEnum misc_menu({"Misc"});
static CatVarBool nopush(misc_menu, "nopush", false, "No Push");
static CatVarBool fullbright(misc_menu, "fullbright", false, "Fullbright");

static void WorldTick() {
// Nopush
#if defined(NEKO_TF2)
    static auto cvar_no_push =
        iface::cvar->FindVar("tf_avoidteammates_pushaway");
    if (nopush == cvar_no_push->GetBool()) cvar_no_push->SetValue(!nopush);
#endif

    // Fullbright
    static auto cvar_fullbright = iface::cvar->FindVar("mat_fullbright");
    if (fullbright != cvar_fullbright->GetBool())
        cvar_fullbright->SetValue(fullbright);
}

void Init() { events::world.Listen(WorldTick); }

}  // namespace modules::source::features::misc
