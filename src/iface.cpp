
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

#include <cstring>
#include <hack/sharedlibrary.hpp>

#include "iface.hpp"

namespace neko::hook::iface {
using namespace neko::hack;
using CreateIFace_f = void* (*)(const char*, int*);

template<typename Type>
Type* BFInterface(CreateIFace_f Ci, std::string_view name) {
    char iface_name[64];
    strncpy(iface_name, name.data(), name.size());
    for (int i = 0; i < 999; i++) {
        sprintf(&iface_name[name.size()], "%03i", i);
        Type* out = reinterpret_cast<Type*>(Ci(iface_name, nullptr));
        if (out)
            return out;
    }
    throw std::runtime_error("can't create interface: " + std::string(name));
}

sourcesdk::BaseClient* client;
sourcesdk::EntityList* entity_list;
void Init(){
    CreateIFace_f CreateIFace;

    SharedLibrary clientso("client");
    clientso.ForceInit();
    CreateIFace = clientso.GetSym<CreateIFace_f>("CreateInterface");
    client = BFInterface<sourcesdk::BaseClient>(CreateIFace, "VClient");
    entity_list = BFInterface<sourcesdk::EntityList>(CreateIFace, "VClientEntityList");
}

}
