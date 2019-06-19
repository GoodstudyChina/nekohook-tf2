
/*
 *	Init file for the source engine module
 *	Please keep everything in the module in its own namespace!
 *
 */

#include "../../util/logging.hpp"  // So we can log

// Internal module stuff
#include "features/features.hpp"
#include "framework/framework.hpp"
#include "hacks/hooks.hpp"
#include "hacks/ifaces.hpp"
#include "util/util.hpp"
// Potentially loadable modules for source
#include "util/surface.hpp"
#if defined(__linux__)
#include "../xlib/input.hpp"
#endif

#include "init.hpp"

namespace modules::source {

// The startup function for the module
void Init() {
    debug_log.Puts("Begin Source engine module init...");

    debug_log.Puts("Finding interfaces...");
    iface::Init();

    debug_log.Puts("Loading Framework Interfaces...");
    framework::Init();

    debug_log.Puts("Loading Modular Cheats...");
    features::Init();

    debug_log.Puts("Loading Util...");
    util::Init();

    // Put possible graphics modules here.
    debug_log.Puts("Loading graphics module...");
    valvesurface::Init();

    // Input modules
    debug_log.Puts("Loading input module...");
#if defined(__linux__)
    xlib::InitInput();
#endif

    // NOTE: Hooking is done last as we let everything set itself up before we
    // allow the game to do anything to avoid data races
    debug_log.Puts("Hooking!");
    hooks::Init();

    debug_log.Puts("Source engine module init!");
}

}  // namespace modules::source
