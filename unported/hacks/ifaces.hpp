
/*
 *
 *	Header file for giving our interfaces to the module.
 *
 */

#pragma once

#include "../sdk/sdk.hpp"
#include "sharedobj.hpp"

namespace modules::source::iface {

// Interfaces themselves
// Essential
extern IBaseClientDLL* client;
extern IClientEntityList* entity_list;
extern IVEngineClient013* engine;
extern IVModelInfoClient* model_info;
extern IEngineTrace* trace;
extern ICvar* cvar;
extern CGlobalVarsBase* global_vars;
// Visual
extern IPanel* panel;
extern IEngineTool* engine_tool;
extern ISurface* surface;

void Init();

}  // namespace modules::source::iface
