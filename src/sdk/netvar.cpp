
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

#include <cassert>
#include <stdexcept>

#include "../iface.hpp"

#include "netvar.hpp"

namespace sourcesdk::netvar {

static std::ptrdiff_t GetOffset(std::initializer_list<std::string_view> var_map){
    assert(var_map.size() >= 2);

    // Find our first argument
    RecvTable* table = nullptr;
    for (ClientClass* i = iface::client->GetAllClasses(); i; i = i->next) {
        if (i->table->name == *var_map.begin()) {
            table = i->table;
            break;
        }
    }
    if (!table)
        throw std::runtime_error("Netvar: not found: " + std::string(*var_map.begin()));


    // Continue recursing through the tree, this should iterate down deeper
    uint32_t cur_offset = 0;
    int level = 1;
    for(int i = 0; i < table->size; i++) {
        const RecvProp& prop = table->props[i];
        // Check if the tree is ours
        if (isdigit(prop.name[0]) || prop.name != *(var_map.begin() + level))
            continue;

        // This is our next tree, iterate stuff
        cur_offset += prop.offset;
        level++;

        // We finished, return the offset
        if (level == var_map.size())
            return cur_offset;

        // Since we arent finished, we setup for another recursion
        table = prop.table;
        i = -1;
    }
    std::string err = "Netvar: cannot find: ";
    for (int i = 0; i < level; i++)
        err += '\"' + std::string(*(var_map.begin() + i)) + "\", ";
    throw std::runtime_error(err);
}

void Init() {
    origin.Set(GetOffset({"DT_BaseEntity", "m_vecOrigin"}));
}


}
