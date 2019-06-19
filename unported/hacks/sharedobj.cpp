
/*
 *
 *	l4d2's shared objects
 *
 */

#include "../../../util/logging.hpp"  // So we can log errors
#include "../../../util/platform.hpp"

#include "sharedobj.hpp"

void* SourceSo::CreateIFace(const char* iface) {
    // TODO: Move this to init
    // Check for the fptr, if not we must find it
    if (!this->fptr) {
        this->fptr =
            reinterpret_cast<CreateIFace_t>(this->GetSym("CreateInterface"));
        if (!this->fptr)
            debug_log.Fmt("Failed to create interface factory for %s",
                          this->GetFileName().c_str());
        else
            debug_log.Fmt("Found interface factory at 0x%x for %s", this->fptr,
                          this->GetFileName().c_str());
    }
    // We have it now so use it
    return fptr(iface, nullptr);
}

namespace modules::source::sharedobj {

// Create move hook + controling attack and movement
SourceSo client("client");

SourceSo server("server");
// For player names
SourceSo engine("engine");
// Used for surface drawing
SourceSo vgui2("vgui2");
// CVARS
SourceSo vstdlib(Platform("libvstdlib.so", "vstdlib.dll"));

// Used for our paint traverse hook
SourceSo vguimatsurface("vguimatsurface");

}  // namespace modules::source::sharedobj
