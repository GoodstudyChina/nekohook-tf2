
/*
 *
 *	The source engine modules file for interfacting with the game and its
 *values
 *
 */

#include <sstream>

#include "../../../hacks/signature.hpp"
#include "../../../util/logging.hpp"  // So we can log errors
#include "sharedobj.hpp"  // We get our interfaces from inside these objects

#include "ifaces.hpp"

namespace modules::source::iface {

// Essential
IBaseClientDLL* client =
    nullptr;
IClientEntityList* entity_list =
    nullptr;                          // Entity Information, pretty much needed
IVEngineClient013* engine = nullptr;  // Currently used for player names and a
                                      // few smaller essential things
IVModelInfoClient* model_info = nullptr;  // Aimboat
IEngineTrace* trace = nullptr;            // Vischecks
ICvar* cvar = nullptr;                    // Source engines cvar system
CGlobalVarsBase* global_vars = nullptr;   // Curtime... Thats it... Welp...
// Visual
IPanel* panel = nullptr;  // Our paint traverse hook and everything to do with
                          // drawing and panels
IEngineTool* engine_tool = nullptr;  // wts
ISurface* surface = nullptr;         // Used for surface drawing

template <typename T>
static T* BfInterface(const char* name, SourceSo& object, int start = 0) {
    // Copy this now, we wont need to later
    char iface_name[128];
    strcpy(iface_name, name);

    size_t len = strlen(name);
    for (int i = start; i < 100; i++) {
        // We can write directly to the end of the base
        sprintf(&iface_name[len], "%03i", i);

        // Try to get interface
        auto result = reinterpret_cast<T*>(object.CreateIFace(iface_name));
        if (result) return result;
    }
    debug_log.Fmt("RIP Software: can't create interface %s!", name);
    return nullptr;
}

void Init() {
    debug_log.Fmt("Client");
    client = BfInterface<IBaseClientDLL>("VClient", sharedobj::client);
    entity_list =
        BfInterface<IClientEntityList>("VClientEntityList", sharedobj::client);
    engine = BfInterface<IVEngineClient013>("VEngineClient", sharedobj::engine);
    model_info =
        BfInterface<IVModelInfoClient>("VModelInfoClient", sharedobj::engine);
    trace = BfInterface<IEngineTrace>("EngineTraceClient", sharedobj::engine);
    cvar = BfInterface<ICvar>("VEngineCvar", sharedobj::vstdlib);
    engine_tool = BfInterface<IEngineTool>("VENGINETOOL", sharedobj::engine);
    // Kill me, WHY DOES THIS BREAK RREEEEEEEE
    // global_vars = **GetSignature<CGlobalVarsBase***>(sharedobj::client, "8B
    // 15 ? ? ? ? F3 0F 10 88 D0 08 00 00", 2);
    global_vars = new CGlobalVarsBase(true);

    panel = BfInterface<IPanel>("VGUI_Panel", sharedobj::vgui2);
    surface = BfInterface<ISurface>("VGUI_Surface", sharedobj::vguimatsurface);
}

}  // namespace modules::source::iface
