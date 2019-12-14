
/*
 * Nekohook: Free, fast and modular cheat software
 * Copyright (C) 2018 Rebekah Rowe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <thread>
#include <fstream>

#include <hack/vfthook.hpp>
#include <neko/hook/events.hpp>

#include "../sdk/usercmd.hpp"
#include "../iface.hpp"
#include "../classid.hpp"
#include "offsets.hpp"

#include "clientmode.hpp"

namespace neko::hook::clientmode {
using CreateMove_f = bool (*)(void*, float, sourcesdk::UserCmd*);

hack::VFTHook clientmode;

static bool CreateMoveHook(void* thisptr, float inputSample, sourcesdk::UserCmd* cmd) {
    const CreateMove_f original = clientmode.GetOriginal<CreateMove_f>(offsets::CreateMove);
    if (!cmd || !iface::engine->IsInGame())
        return original(thisptr, inputSample, cmd);     
    events::World();
    return original(thisptr, inputSample, cmd);
}

// To get client classes first without speed penalty
static bool StarterCMHook(void* thisptr, float inputSample, sourcesdk::UserCmd* cmd) {

    if (cmd && iface::engine->IsInGame()) {
        ClassId::Init();
        clientmode.Hook(CreateMoveHook, offsets::CreateMove);
    }

    const CreateMove_f original = clientmode.GetOriginal<CreateMove_f>(offsets::CreateMove);
    return original(thisptr, inputSample, cmd);
}

void Init(){
    std::thread t([](){
        uintptr_t* tmp;
        while (!(
            tmp = **(uintptr_t***)(((*(uintptr_t**)iface::client)[10]) + 1))) {
        }
        clientmode.Init(tmp);
        clientmode.Hook(StarterCMHook, offsets::CreateMove);
        clientmode.Finish();
    });
    t.detach();
}
}
